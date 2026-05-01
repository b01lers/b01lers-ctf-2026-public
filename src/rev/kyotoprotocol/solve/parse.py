import json
import os
import re
import traceback

import colorama


def parse_local_offset(string):
    local_fmt = r"^\*\((?:_DWORD|int|unsigned int)\s*\*\)\(v0\s*-\s*([0-9]+)\)$"
    m = re.match(local_fmt, string.strip())
    if m:
        offset = int(m.group(1))
        return offset
    return None


def print_expr(obj):
    match obj["type"]:
        case "number":
            return obj["value"]
        case "binary_op":
            return f"({print_expr(obj['left'])} {obj['operation']} {print_expr(obj['right'])})"
        case "env_str":
            return f"env_str({obj['name']})"
        case "env":
            return f"env({obj['name']})"
        case "arg":
            return f"arg{obj['position']}"
        case _:
            return None


def print_expr_array(exprs):
    return [print_expr(x) for x in exprs]


def resolve_expr(string, local_args):
    # number
    num_fmt = r"^0x([0-9a-fA-F]+)$"
    m = re.match(num_fmt, string.strip())
    if m:
        return {"type": "number", "value": int(m.group(1), 16)}
    # simple number
    num_fmt = r"^([0-9]+)$"
    m = re.match(num_fmt, string.strip())
    if m:
        return {"type": "number", "value": int(m.group(1))}

    # decrement
    if string.strip().startswith("--"):
        inner = string.strip()[2:]
        inner_expr = resolve_expr(inner, local_args)
        return {"type": "binary_op", "operation": "-", "left": inner_expr, "right": {"type": "number", "value": 1}}

    # increment
    if string.strip().startswith("++"):
        inner = string.strip()[2:]
        inner_expr = resolve_expr(inner, local_args)
        return {"type": "binary_op", "operation": "+", "left": inner_expr, "right": {"type": "number", "value": 1}}

    # envvar
    env_fmt = r"^getenv\(\"([a-zA-Z0-9_]+)\"\)$"
    m = re.match(env_fmt, string.strip())
    if m:
        return {"type": "env_str", "name": m.group(1)}

    # atoi
    atoi_fmt = r"^atoi\((.+)\)$"
    m = re.match(atoi_fmt, string.strip())
    if m:
        inner = m.group(1).strip()
        inner_expr = resolve_expr(inner, local_args)
        assert inner_expr["type"] == "env_str", "atoi only supports env vars"
        return {"type": "env", "name": inner_expr["name"]}

    # binary operation
    for op in ["+", "-", "*", "/", "%"]:
        if op in string:
            start = 0
            while True:
                # we just try splitting everywhere
                idx = string.find(op, start)
                if idx == -1:
                    break
                left = string[:idx].strip()
                right = string[idx + 1 :].strip()
                try:
                    left_expr = resolve_expr(left, local_args)
                    right_expr = resolve_expr(right, local_args)
                    return {"type": "binary_op", "operation": op, "left": left_expr, "right": right_expr}
                except Exception:
                    start = idx + 1
                    continue

    # local rbp var

    local_offset = parse_local_offset(string)
    if local_offset is not None:
        if local_offset not in local_args["_rbp"]:
            raise Exception(f"Offset {local_offset} not found in local args")
        return local_args["_rbp"][local_offset]

    if string in local_args["locals"]:
        return local_args["locals"][string]

    raise Exception(f"Unknown argument format: {string}")


def parse_write_line(line):
    next_fmt = r".*\.\/chall \$LINENO (-?[0-9]+)"
    if "write_line(v0" in line:
        return {
            "type": "none",
        }
    elif line.startswith('write_line("echo'):
        string = r"write_line\(\"((?:\\.|[^\"\\])*)\""
        m = re.match(string, line)
        assert m, "Failed to parse write_line echo format"
        assert m.group(1).startswith("echo "), "Expected write_line to start with echo"
        return {
            "type": "write",
            "line": m.group(1)[5:].replace('\\"', '"').replace("\\\\", "\\"),
        }
    elif line.startswith('write_line("read -n 100 input"'):
        return {
            "type": "read",
        }
    elif line.startswith('write_line("export input"'):
        return {
            "type": "none",
        }
    elif m := re.match(next_fmt, line.strip()):
        string = line[line.index("write_line(") + len("write_line(") : line.rindex(",")].strip()
        assert string.startswith('"./chall $LINENO'), "Expected write_line to start with ./chall $LINENO"
        args = string[1:-1].split(" ")[3:]
        parsed_args = []
        for arg in args:
            assert re.match(r"-?[0-9]+", arg), "Expected write_line args to be numbers"
            parsed_args.append({"type": "number", "value": int(arg)})

        return {
            "type": "next",
            "next": int(m.group(1)),
            "args": parsed_args,
        }
    else:
        raise Exception("Unknown write_line format")


def parse_snprintf(line, local_args):
    # write line
    args = line[len("snprintf(") : -2].split(",")
    assert args[0].strip().lower() == "(char *)(v0 - 256)"
    assert args[1].strip().lower().startswith("0xc8")
    # print("write:", args[2:])
    fmtstr = args[2].strip()[1:-1]
    # export
    export_fmt = r"export ([a-zA-Z0-9_]+)=%d"
    m = re.match(export_fmt, fmtstr)
    if m:
        assert len(args) == 4
        out = resolve_expr(args[3], local_args)
        # print(colorama.Fore.GREEN + "export:", m.group(1), "=", out)
        return {
            "type": "export",
            "name": m.group(1),
            "value": out,
        }
    # next instruction
    next_fmt = r"\.\/chall \$LINENO (-?[0-9]+)"
    m = re.match(next_fmt, fmtstr)
    if m:
        fmt_str = line[
            line.index("0xC8u,") + len("0xC8u,") : line.index(",", line.index("0xC8u,") + len("0xC8u,"))
        ].strip()

        assert fmt_str.startswith('"./chall $LINENO'), "Expected snprintf to start with ./chall $LINENO"
        # branch node
        if len(re.findall("./chall", fmt_str)) == 3:
            print(fmt_str)
            branch_fmt = r"((?:\\.|[^\"\\])*) && ((?:\\.|[^\"\\])*) \|\| ((?:\\.|[^\"\\])*)"
            m = re.match(branch_fmt, fmt_str[1:-1])
            assert m, "Failed to parse branch snprintf format"
            print(colorama.Fore.YELLOW + "Branch condition:", m.group(1))
            print(colorama.Fore.YELLOW + "True target:", m.group(2))
            print(colorama.Fore.YELLOW + "False target:", m.group(3))

            supplied = args[3:]

            def parse_fmt(fmt):
                fmt_args = fmt.split(" ")[3:]
                next_id = int(fmt.split(" ")[2])
                parsed_args = []
                for arg in fmt_args:
                    if arg[0] == "%":
                        supplied_arg = supplied.pop(0).strip()
                        parsed_args.append(resolve_expr(supplied_arg, local_args))
                        continue
                    assert re.match(r"-?[0-9]+", arg), "Expected snprintf args to be numbers"
                    parsed_args.append({"type": "number", "value": int(arg)})
                return {
                    "next": next_id,
                    "args": parsed_args,
                }

            condition = parse_fmt(m.group(1))
            true_target = parse_fmt(m.group(2))  # &&, nonzero
            false_target = parse_fmt(m.group(3))  # ||, zero

            # return {
            #     "type": "return_code_branch",
            #     "condition": parse_fmt(m.group(1)),
            #     "true_branch": parse_fmt(m.group(2)),
            #     "false_branch": parse_fmt(m.group(3)),
            # }
            # the two times the condition node is used, it's just exit with args[0] so this is just a jz/jnz
            return {
                "type": "branch",
                "condition": {
                    "left": condition["args"][0],
                    "operation": "==",
                    "right": {"type": "number", "value": 0},
                },
                "true_target": true_target,
                "false_target": false_target,
            }

        parsed_args = []
        fmt_args = fmt_str[1:-1].split(" ")[3:]
        supplied = args[3:]
        for arg in fmt_args:
            if arg[0] == "%":
                supplied_arg = supplied.pop(0).strip()
                parsed_args.append(resolve_expr(supplied_arg, local_args))
                continue
            assert re.match(r"-?[0-9]+", arg), "Expected snprintf args to be numbers"
            parsed_args.append({"type": "number", "value": int(arg)})

        # for arg in args[3:]:
        #     arg = arg.strip()
        #     parsed_args.append(resolve_expr(arg, local_args))
        # print(colorama.Fore.GREEN + "next:", m.group(1), parsed_args)
        return {
            "type": "next",
            "next": int(m.group(1)),
            "args": parsed_args,
        }
    raise Exception("Unknown snprintf format")


def parse_generic_write(line, local_args):
    if "write_line" in line:
        return parse_write_line(line)
    elif "snprintf(" in line:
        return parse_snprintf(line, local_args)


def parse_assign(line, local_args):
    var, expr = line.split("=", 1)
    assert "=" not in var
    assert "=" not in expr
    var = var.strip()
    expr = expr.strip()
    if expr.endswith(";"):
        expr = expr[:-1]
    out = resolve_expr(expr, local_args)
    offset = parse_local_offset(var)
    if offset is not None:
        local_args["_rbp"][offset] = out
        # print(colorama.Fore.LIGHTBLUE_EX + "assign:", offset, "=", out)
    else:
        local_args["locals"][var] = out
        # print(colorama.Fore.LIGHTBLUE_EX + "assign:", var, "=", out)


def parse_condition(cond, local_args):
    if ">" not in cond and "<" not in cond and "=" not in cond and "!" not in cond:
        # != 0
        return {
            "left": resolve_expr(cond, local_args),
            "operation": "!=",
            "right": {"type": "number", "value": 0},
        }
    else:
        ops = [">", "<", ">=", "<=", "==", "!="]
        for op in ops:
            if op in cond:
                try:
                    left, right = cond.split(op, 1)
                    left = resolve_expr(left, local_args)
                    right = resolve_expr(right, local_args)
                    return {
                        "left": left,
                        "operation": op,
                        "right": right,
                    }
                except Exception:
                    continue
                break
        else:
            raise Exception("Unknown branch condition format")


def parse(id):
    out_ops = []
    data = open(f"dumped_c/{id}.c", "r").readlines()
    data = [line.strip() for line in data]

    # truncate until first newline, last }
    data = data[data.index("") + 1 :]
    assert data[-1] == "}"
    # assert data[-2] == "close_and_exit(0);"
    data = data[:-1]
    new_dat = "\n".join(data)

    # consolidate all function calls into one line
    new_dat = new_dat.replace("\n", "")
    new_dat = new_dat.replace(";", ";\n")
    new_dat = new_dat.replace("{", "{\n")
    new_dat = new_dat.replace("}", "}\n")

    local_args = {"_rbp": {}, "locals": {}}

    arg0_offset = 17872
    for i in range(1, 9):
        local_args["_rbp"][arg0_offset - (i - 1) * 4] = {"type": "arg", "position": i - 1}

    # parse line by line
    lines = new_dat.splitlines()

    # remove all lines including sub_A762, sub_A666, dword_ABE98
    lines = [
        line
        for line in lines
        if not line.strip().startswith("sub_A762(")
        and not line.strip().startswith("sub_A666(")
        and "dword_ABE98" not in line
    ]

    print("\n".join(lines))

    line_idx = 0
    while line_idx < len(lines):
        line = lines[line_idx].strip()
        if line.startswith("write_header("):
            pass
        elif line.startswith("write_line"):
            out = parse_write_line(line)
            if out["type"] == "write":
                print(colorama.Fore.GREEN + "Write:", out["line"])
                out_ops.append(out)
            elif out["type"] == "next":
                print(colorama.Fore.GREEN + "Next:", out["next"], "with args", print_expr_array(out.get("args", [])))
                out_ops.append(out)
            elif out["type"] == "none":
                pass
            elif out["type"] == "read":
                print(colorama.Fore.GREEN + "Read input")
                out_ops.append(out)
            else:
                raise Exception("Unknown write_line type")
        elif line.startswith("close_and_exit("):
            pass
            # arg = line[len("close_and_exit(") : -2].strip()
            # exit_expr = resolve_expr(arg, local_args)
            # print(colorama.Fore.GREEN + "Exit with code", print_expr(exit_expr))
        elif line.startswith("snprintf("):
            # write line
            out = parse_snprintf(line, local_args)
            if out["type"] == "export":
                print(colorama.Fore.GREEN + "Export:", out["name"], "=", print_expr(out["value"]))
                out_ops.append(out)
            elif out["type"] == "next":
                print(colorama.Fore.GREEN + "Next:", out["next"], "with args", print_expr_array(out.get("args", [])))
                out_ops.append(out)
            elif out["type"] == "branch":
                print(
                    colorama.Fore.LIGHTBLUE_EX + "Branch on",
                    f"({print_expr(out['condition']['left'])} {out['condition']['operation']} {print_expr(out['condition']['right'])})",
                )
                print(
                    colorama.Fore.GREEN
                    + f"true target: {out['true_target']['next']} with args {print_expr_array(out['true_target'].get('args', []))}"
                )
                print(
                    colorama.Fore.GREEN
                    + f"false target: {out['false_target']['next']} with args {print_expr_array(out['false_target'].get('args', []))}"
                )
                out_ops.append(out)
            else:
                raise Exception("Unknown snprintf type")
        elif line.startswith("if"):
            # getenv ternary
            if "getenv" in line and lines[line_idx + 1].strip().startswith("else"):
                # if ( getenv("g_8511") )v2 = getenv("g_8511");
                fmt = r"if\s*\(\s*getenv\(\s*\"([a-zA-Z0-9_]+)\s*\"\)\s*\)(\s*[^;]+);"
                m = re.match(fmt, line.strip())
                assert m, "Failed to parse getenv ternary"
                parse_assign(m.group(2), local_args)
                line_idx += 1

            # branch
            elif ")snprintf" in line:
                # if ( *(int *)(v0 - 17864) <= 57 )snprintf(
                trimmed = line.replace(" ", "")
                cond = trimmed[trimmed.index("if(") + 3 : trimmed.index(")snprintf")].strip()
                # print(colorama.Fore.YELLOW + "branch on:", cond)
                condition = parse_condition(cond, local_args)
                print(
                    colorama.Fore.LIGHTBLUE_EX + "Branch on",
                    f"({print_expr(condition['left'])} {condition['operation']} {print_expr(condition['right'])})",
                )
                true_block = line[line.index(")snprintf") + 1 :].strip()
                false_block = lines[line_idx + 1].strip()[len("else") :].strip()
                true_target = parse_generic_write(true_block, local_args)
                false_target = parse_generic_write(false_block, local_args)
                assert true_target is not None and false_target is not None, "Failed to parse branch targets"
                print(
                    colorama.Fore.GREEN
                    + f"true target: {true_target['next']} with args {print_expr_array(true_target.get('args', []))}"
                )
                print(
                    colorama.Fore.GREEN
                    + f"false target: {false_target['next']} with args {print_expr_array(false_target.get('args', []))}"
                )
                out_ops.append(
                    {"type": "branch", "condition": condition, "true_target": true_target, "false_target": false_target}
                )
                line_idx += 1
            elif (
                "{" in line
                and lines[line_idx + 2].strip() == "}"
                and lines[line_idx + 3].strip() == "else{"
                and lines[line_idx + 5].strip() == "}"
            ):
                """
              if ( *(_DWORD *)(v0 - 17872) == 34 ){
                write_line("./chall $LINENO 2354", (unsigned int)(*(_DWORD *)(v0 - 17932) + v1));
              }else{
                write_line("./chall $LINENO 4099", (unsigned int)(*(_DWORD *)(v0 - 17932) + v1));
              }
              decomp moment
              """
                cond_fmt = r"^if\s*\(\s*(.+)\s*\)\s*{$"
                m = re.match(cond_fmt, line.strip())
                assert m, "Failed to parse if condition"
                cond = m.group(1).strip()
                condition = parse_condition(cond, local_args)
                print(
                    colorama.Fore.LIGHTBLUE_EX + "Branch on",
                    f"({print_expr(condition['left'])} {condition['operation']} {print_expr(condition['right'])})",
                )
                true_block = lines[line_idx + 1].strip()
                false_block = lines[line_idx + 4].strip()
                # print(colorama.Fore.YELLOW + "true block:", true_block)
                # print(colorama.Fore.YELLOW + "false block:", false_block)
                true_target = parse_generic_write(true_block, local_args)
                false_target = parse_generic_write(false_block, local_args)
                assert true_target is not None and false_target is not None, "Failed to parse branch targets"
                print(
                    colorama.Fore.GREEN
                    + f"true target: {true_target['next']} with args {print_expr_array(true_target.get('args', []))}"
                )
                print(
                    colorama.Fore.GREEN
                    + f"false target: {false_target['next']} with args {print_expr_array(false_target.get('args', []))}"
                )
                out_ops.append(
                    {"type": "branch", "condition": condition, "true_target": true_target, "false_target": false_target}
                )
                line_idx += 5
            else:
                print(colorama.Fore.YELLOW + "OTHER IF:", line)
                raise Exception("Unknown if format")

        elif "= " in line:
            # assignment line

            idx = line.index("=")
            op = line[idx - 1]
            if op != " ":
                print(colorama.Fore.YELLOW + "OTHER ASSIGNMENT:", line)
                var, expr = line.split("=", 1)
                assert "=" not in var
                assert "=" not in expr
                var = var[:-1].strip()
                expr = expr.strip()
                if expr.endswith(";"):
                    expr = expr[:-1]
                offset = parse_local_offset(var)
                if offset is not None:
                    local_args["_rbp"][offset] = {
                        "type": "binary_op",
                        "operation": op,
                        "left": local_args["_rbp"][offset],
                        "right": resolve_expr(expr, local_args),
                    }
                    print(
                        {
                            "type": "binary_op",
                            "operation": op,
                            "left": local_args["_rbp"][offset],
                            "right": resolve_expr(expr, local_args),
                        }
                    )
                else:
                    local_args["locals"][var] = {
                        "type": "binary_op",
                        "operation": op,
                        "left": local_args["locals"].get(var, {"type": "number", "value": 0}),
                        "right": resolve_expr(expr, local_args),
                    }
                    assert False, "ow"
            parse_assign(line, local_args)
        else:
            print(colorama.Fore.YELLOW + "OTHER:", line)
            raise Exception("Unknown line format")
        print(colorama.Fore.RESET, end="")
        line_idx += 1
    print(out_ops)
    return out_ops


dir = "dumped_c/"


files = os.listdir(dir)
files.sort(key=lambda x: int(x[:-2]))

out_data = {}

for fname in files:
    if fname.endswith(".c"):
        id = fname[:-2]
        print(colorama.Fore.CYAN + f"Parsing function {id}")
        print(colorama.Fore.RESET, end="")
        if id == "6917":
            # decrypt & write flag
            out_data["6917"] = [{"type": "decrypt", "next": 5237}]
            continue
        if id == "8408":
            # check
            # correct: 8234
            # incorrect: 9806
            out_data["8408"] = [
                {"type": "check", "correct": {"next": 8234, "args": []}, "incorrect": {"next": 9806, "args": []}}
            ]
            continue
        if id == "8275":
            # write input to vars
            # out = 8947
            out_data["8275"] = [
                {
                    "type": "input",
                },
                {"type": "next", "next": 8947, "args": []},
            ]
            continue
        try:
            out_data[id] = parse(id)
        except Exception as e:
            print(f"Error parsing function {id}: {e}")
            traceback.print_exc()
            file_content = open(os.path.join(dir, fname), "r").read()
            print("File content:")
            print(file_content)
            break
        print("")


open("parsed.json", "w").write(json.dumps(out_data))
