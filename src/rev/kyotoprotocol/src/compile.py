import os
import random
import re

random.seed(859623)

obfuscate_options = {
    "change_ids": True,
    "shuffle_ops": True,
    "shuffle_blocks": True,
    "rename_globals": True,
}

with open("program.kyoto", "r") as f:
    program_code = f.read().splitlines()

output = []
label_map = {}
global_map = {}

blocks = []
block = None


def write_line(line):
    if block is not None and obfuscate_options["shuffle_blocks"]:
        block.append(line)
        return
    # print(f'\033[90m{line}\x1b[0m')
    output.append(line)


D_PROG = "./chall $LINENO"


def get_global_name(name):
    if re.match(r"^i(\d+)$", name):
        return name
    if obfuscate_options["rename_globals"]:
        if name not in global_map:
            new_name = f"g_{random.randint(1000, 9999)}"
            global_map[name] = new_name
        return global_map[name]
    return name


def parse_arg(arg):
    if arg.startswith('"') and arg.endswith('"'):
        return ("string", arg[1:-1])
    elif arg.startswith("@"):
        return ("var", int(arg[1:]))
    elif arg.startswith("$"):
        return ("mem", arg[1:])
    else:
        return ("int", int(arg))


def parse_line(line):
    opcode = ""
    i = 0
    while i < len(line) and line[i] != " " and line[i] != ";":
        opcode += line[i]
        i += 1

    args = []
    while i < len(line):
        while i < len(line) and line[i] == " ":
            i += 1
        if i >= len(line):
            break
        if line[i] == ";":
            break
        arg = ""
        if line[i] == '"':
            i += 1
            while i < len(line) and line[i] != '"':
                arg += line[i]
                i += 1
            i += 1  # skip closing quote
            arg = '"' + arg + '"'
        else:
            while i < len(line) and line[i] != " " and line[i] != ";":
                arg += line[i]
                i += 1
        args.append(parse_arg(arg))

    i += 1

    stack = []
    while i < len(line):
        while i < len(line) and line[i] == " ":
            i += 1
        if i >= len(line):
            break
        arg = ""
        if line[i] == '"':
            i += 1
            while i < len(line) and line[i] != '"':
                arg += line[i]
                i += 1
            i += 1  # skip closing quote
            arg = '"' + arg + '"'
        else:
            while i < len(line) and line[i] != " " and line[i] != ";":
                arg += line[i]
                i += 1
        stack.append(parse_arg(arg))

    return opcode, args, stack


def get_arg_value(arg):
    if arg[0] == "int":
        return str(arg[1])
    elif arg[0] == "var":
        return f"stack[{arg[1] - 1}]"
    elif arg[0] == "string":
        return f'"{arg[1]}"'
    elif arg[0] == "mem":
        return f'atoi(getenv("{get_global_name(arg[1])}") ? getenv("{get_global_name(arg[1])}") : "0")'


def format_args(f_args):
    if len(f_args) == 0:
        return ""
    else:
        return f", {', '.join(f_args)}"


def construct_template(parts, nospace=False):
    f_str = []
    f_args = []
    for part in parts:
        if isinstance(part, str):
            f_str.append(part)
        elif isinstance(part, int):
            f_str.append(str(part))
        elif isinstance(part, tuple):
            if part[0] == "int":
                f_str.append(str(part[1]))
            elif part[0] == "var":
                f_str.append("%d")
                f_args.append(f"stack[{part[1] - 1}]")
            elif part[0] == "string":
                f_str.append(f'\\"{part[1]}\\"')
            elif part[0] == "mem":
                f_str.append("%d")
                if obfuscate_options["rename_globals"]:
                    f_args.append(
                        f'atoi(getenv("{get_global_name(part[1])}") ? getenv("{get_global_name(part[1])}") : "0")'
                    )
            elif part[0] == "tmp":
                f_str.append("%d")
                f_args.append("tmp")
        else:
            print("invalid template ", part)
            exit(1)
    delim = "" if nospace else " "
    if len(f_args) == 0:
        write_line(f'\twrite_line("{delim.join(f_str)}", line_no + (w_line++));')
    else:
        write_line(f"""
\tsnprintf(output, 200, \"{delim.join(f_str)}\"{format_args(f_args)});
\twrite_line(output, line_no + (w_line++));
""")


def assign(target, *parts):
    expr = []
    for part in parts:
        if isinstance(part, str):
            expr.append(part)
        elif isinstance(part, tuple):
            expr.append(get_arg_value(part))
        else:
            print("invalid template ", part)
            exit(1)
    if target[0] == "var":
        write_line(f"\t{get_arg_value(target)} = {' '.join(expr)};")
    elif target[0] == "mem":
        write_line(f"\ttmp = {' '.join(expr)};")
        construct_template(
            ["export ", get_global_name(target[1]), "=", ("tmp",)], nospace=True
        )


def mark_stack(stack, *target):
    for var in target:
        if var[0] == "var":
            while len(stack) < var[1]:
                stack.append(("var", len(stack) + 1))


def resolve_jmp_target(arg):
    if arg[0] == "string":
        if arg[1] in label_map:
            return label_map[arg[1]]
        else:
            print("invalid label ", arg[1])
            exit(1)
    if arg[0] == "int":
        return line_id_map[arg[1] - 1]
    return arg


line_id_map = {}
id_line_map = {}

ID = 0
ID_POOL = set()


def create_id():
    if not obfuscate_options["change_ids"]:
        global ID
        ID += 1
        return ID - 1
    global ID_POOL
    new_id = random.randint(1, 9999)
    while new_id in ID_POOL:
        new_id = random.randint(1, 9999)
    ID_POOL.add(new_id)
    return new_id


def get_line_id(line):
    if line in line_id_map:
        return line_id_map[line]
    else:
        id = create_id()
        line_id_map[line] = id
        id_line_map[id] = line
        return id


expected_arg_map = {}
flow_queue = [(None, 0)]  # (predecessor id, line_idx)


# assign ids to each asm line
for i in range(len(program_code)):
    line = program_code[i].strip()
    match = re.search(r"<(.+)>", line)
    if "//" in line:
        line = line[: line.index("//")]
        line = line.strip()
    get_line_id(i)
    if match:
        label_map[match.group(1)] = line_id_map[i]

# testval exit payloads
testval_ids = []

# read payload
read_ids = []

write_line(f"""
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

unsigned int w_line = 0;
int write_line(char *str, int pos);
void revert_file();
void clear_before(unsigned int line);
void clear_before_n(unsigned int line, int n);
void leave(int code);
void decode_frame(int input_idx);
const char flag[] = "\\x04\\x97\\xbe\\xc2\\xfa\\x5e\\xf5\\x59\\x5d\\xc1\\x94\\x00\\x2e\\xb8\\x74\\xe7\\x41\\x1c\\xc9\\x01\\x98\\x2f\\xb3\\xc7\\xfa\\x84\\x40\\xc5\\xd6\\x46\\x78\\x13";
const int flag_len = 32;

void do_op(int argc, char** argv){{

  if (argc < 3) {{
    revert_file();
    write_line("int () {{", 1);
    write_line("{D_PROG} {999999}", 2);
    write_line("exit", 3);
    write_line("}}", 4);
    write_line("trap \\"int\\" INT", 5);
    write_line("{D_PROG} {get_line_id(0)}", 6);
    return;
  }}


  char output[200];
  int stack[10] = {{0}};
  int tmp;
  int line_no = atoi(argv[1]) - 1;
  int id = atoi(argv[2]);
  for (int i = 0; i < argc - 3; i++) {{
    stack[i] = atoi(argv[i + 3]);
  }}

  if (line_no > 250 && line_no % 3 == 0) {{
    double frame = (line_no - 250) / 5200.0 * 2190.0;
    if (frame > 0 && frame < 2190) {{
      decode_frame((int) frame);
    }}
  }}
""")

while len(flow_queue) > 0:
    block = []
    predecessor = flow_queue[0][0]
    line_idx = flow_queue.pop(0)[1]
    line_id = get_line_id(line_idx)
    line = program_code[line_idx].strip()
    if "//" in line:
        line = line[: line.index("//")]
        line = line.strip()

    match = re.search(r"<(.+)>", line)
    if match:
        line = line.replace(f"<{match.group(1)}>", "")
        line = line.strip()

    opcode, args, stack = parse_line(line)

    # print(f"Opcode: {opcode}, Args: {args}, Stack: {stack}")

    write_line(f"// {line}")
    write_line(f"inst_{line_id}:\n")
    # expected_args = len(stack)

    # print(f"""
    #         if (argc - 3 < {expected_args}) {{
    #             fprintf(stderr, "Error: Expected {expected_args} arguments, got %%d\\n", argc - 3);
    #             exit(1);
    #         }}
    #         """)

    # parse stack

    next_line = line_id_map.get(line_idx + 1, -1)
    if program_code[line_idx - 1] and (program_code[line_idx - 1].startswith("store")):
        write_line("\tclear_before_n(line_no, 102);")
    elif line_idx == 0:
        write_line("\tclear_before_n(line_no, 7);")
    else:
        write_line("\tclear_before(line_no);")
    # op
    if opcode == "nop":
        construct_template([D_PROG, next_line, *stack])
        pass
    elif opcode == "exit":
        write_line("\trevert_file();")
        write_line("\tleave(0);")
    elif opcode == "echo":
        construct_template(["echo", args[0]])
        construct_template([D_PROG, next_line, *stack])
    elif opcode == "add":
        mark_stack(stack, args[2])
        assign(args[2], args[0], "+", args[1])
        construct_template([D_PROG, next_line, *stack])
    elif opcode == "sub":
        mark_stack(stack, args[2])
        assign(args[2], args[0], "-", args[1])
        construct_template([D_PROG, next_line, *stack])
    elif opcode == "mul":
        mark_stack(stack, args[2])
        assign(args[2], args[0], "*", args[1])
        construct_template([D_PROG, next_line, *stack])
    elif opcode == "div":
        mark_stack(stack, args[2])
        assign(args[2], args[0], "/", args[1])
        construct_template([D_PROG, next_line, *stack])
    elif opcode == "mod":
        mark_stack(stack, args[2])
        assign(args[2], args[0], "%", args[1])
        construct_template([D_PROG, next_line, *stack])
    elif opcode == "set":
        mark_stack(stack, args[0])
        assign(args[0], args[1])
        construct_template([D_PROG, next_line, *stack])
    elif opcode == "jz":
        test_id = create_id()
        testval_ids.append(test_id)
        id_line_map[test_id] = line_idx
        construct_template(
            [
                D_PROG,
                test_id,
                args[0],
                "&&",
                D_PROG,
                resolve_jmp_target(args[1]),
                *stack,
                "||",
                D_PROG,
                next_line,
                *stack,
            ]
        )
    elif opcode == "jnz":
        test_id = create_id()
        testval_ids.append(test_id)
        id_line_map[test_id] = line_idx
        construct_template(
            [
                D_PROG,
                test_id,
                args[0],
                "&&",
                D_PROG,
                next_line,
                *stack,
                "||",
                D_PROG,
                resolve_jmp_target(args[1]),
                *stack,
            ]
        )
    elif opcode == "jl":
        write_line(f"""\tif ({get_arg_value(args[0])} < {get_arg_value(args[1])}) {{""")
        construct_template([D_PROG, resolve_jmp_target(args[2]), *stack])
        write_line("""\t}else{""")
        construct_template([D_PROG, next_line, *stack])
        write_line("\t}")
    elif opcode == "jg":
        write_line(f"""\tif ({get_arg_value(args[0])} > {get_arg_value(args[1])}) {{""")
        construct_template([D_PROG, resolve_jmp_target(args[2]), *stack])
        write_line("""\t}else{""")
        construct_template([D_PROG, next_line, *stack])
        write_line("\t}")
    elif opcode == "je":
        write_line(
            f"""\tif ({get_arg_value(args[0])} == {get_arg_value(args[1])}) {{"""
        )
        construct_template([D_PROG, resolve_jmp_target(args[2]), *stack])
        write_line("""\t}else{""")
        construct_template([D_PROG, next_line, *stack])
        write_line("\t}")
    elif opcode == "jne":
        write_line(
            f"""\tif ({get_arg_value(args[0])} != {get_arg_value(args[1])}) {{"""
        )
        construct_template([D_PROG, resolve_jmp_target(args[2]), *stack])
        write_line("""\t}else{""")
        construct_template([D_PROG, next_line, *stack])
        write_line("\t}")
    elif opcode == "jmp":
        construct_template([D_PROG, resolve_jmp_target(args[0]), *stack])
    elif opcode == "read":
        # read into i0 to i99
        construct_template(["read -n 100 input"])
        construct_template(["export input"])
        construct_template([D_PROG, next_line, *stack])
    elif opcode == "store":
        write_line("""
\tchar* input = getenv("input");
\tif (input == NULL) {
\t  revert_file();
\t  leave(0);
\t}
\tint i = 0;
\tchar output2[10];
\tchar output3[10];
\twhile (*input) {
\t  snprintf(output2, 10, "i%d", i);
\t  snprintf(output3, 10, "%u", (char)(*input));
\t  snprintf(output, 200, "export %s=%s", output2, output3);
\t  write_line(output, line_no + (w_line++));
\t  i++;
\t  input++;
\t}
""")
        construct_template([D_PROG, next_line, *stack])
    elif opcode == "check":
        """
    echo $c0 // 1820085546
    echo $c1 // 1410707190
    echo $c2 // 972076578
    echo $c3 // 1718772620
    echo $rows // 333333333
    echo $cols // 333333333
    echo $squares // 333333333
    034c5e261f2141625a342b4f275d264b4c11592139323234410a18271f3c0a3c253303392761425d5e633a2655503d525e544b1131525950612b50302d5f27271f0a295e2c553d255558474b43262d2749
    """
        # check $c0 $c1 $c2 $c3 $rows $cols $squares "correct" "incorrect"
        write_line(
            f"""\tif ({get_arg_value(args[0])} == 1820085546 && {get_arg_value(args[1])} == 1410707190 && {get_arg_value(args[2])} == 972076578 && {get_arg_value(args[3])} == 1718772620 && {get_arg_value(args[4])} == 333333333 && {get_arg_value(args[5])} == 333333333 && {get_arg_value(args[6])} == 333333333) {{"""
        )
        write_line(
            f'\t\tsprintf(output, "export key={"%02hhx" * 81}", {", ".join([get_arg_value(("mem", f"b{i}")) for i in range(81)])});'
        )
        write_line("""\twrite_line(output, line_no + (w_line++));""")
        write_line(
            """\twrite_line("export key=$(echo -n $key | xxd -r -p | sha256sum | awk '{print $1}')", line_no + (w_line++));"""
        )
        construct_template([D_PROG, resolve_jmp_target(args[7]), *stack])
        write_line("""\t}else{""")
        construct_template([D_PROG, resolve_jmp_target(args[8]), *stack])
        write_line("\t}")
    elif opcode == "decrypt":
        write_line("""
\tchar* key = getenv("key");
\tif (key == NULL) {
\t  revert_file();
\t  leave(0);
\t}
\t char key_hex[32] = {};
\tfor (int i = 0; i < 64; i += 2) {
\t  char byte_str[3] = {key[i], key[i + 1], '\\0'};
\t  key_hex[i / 2] = (char)strtol(byte_str, NULL, 16);
\t}
\tchar decrypted[flag_len+2];
\tfor (int i = 0; i < flag_len; i++) {
\t  decrypted[i] = flag[i] ^ key_hex[i % 32];
\t}
\tdecrypted[flag_len+1] = '\\0';
\tdecrypted[flag_len] = '\\n';
\tFILE *f = fopen("flag.txt", "w");
\tif (f == NULL) {
\t  printf("%s", decrypted);
\t  revert_file();
\t  leave(0);
\t}
\tfwrite(decrypted, sizeof(char), flag_len + 1, f);
\tfclose(f);
""")
        construct_template([D_PROG, next_line, *stack])
    elif opcode == "print_flag":
        construct_template(["echo 'cat flag.txt'"])
        construct_template([D_PROG, next_line, *stack])
    elif opcode == "":
        construct_template([D_PROG, next_line, *stack])

    write_line("\tleave(0);")
    if obfuscate_options["shuffle_blocks"]:
        blocks.append(block)
    if len(program_code) <= line_idx + 1:
        break
    flow_queue.append((line_idx, line_idx + 1))
    # write stack


# write all testvals

for id in testval_ids:
    block = []
    line_idx = id_line_map[id]
    line = program_code[line_idx].strip()
    write_line(f"// testval for line {line_idx}")
    write_line(f"inst_{id}:\n")
    write_line("\tleave(stack[0]);")
    if obfuscate_options["shuffle_blocks"]:
        blocks.append(block)

if obfuscate_options["shuffle_blocks"]:
    random.shuffle(blocks)
    for block in blocks:
        for line in block:
            output.append(line)
block = None

write_line("}")

# jumptable

jumps = list(id_line_map.keys())
if obfuscate_options["shuffle_ops"]:
    random.shuffle(jumps)

jump = f"""
  void *jumptable[] = {{{", ".join(f"&&inst_{id}" for id in jumps)}}};
  int ids[] = {{{", ".join(f"{id}" for id in jumps)}}};
  for (int i = 0; i < {len(jumps)}; i++) {{
    if (ids[i] == id) {{
      goto *jumptable[i];
    }}
  }}
  revert_file();
  leave(0);
"""
output.insert(1, jump)

with open("output.c", "w") as f:
    f.write("\n".join(output))


os.system("gcc -o chall chall.c output.c bad_apple.c -Wall")
os.system("strip chall")
os.system("rm flag.txt 2> /dev/null")
with open("chall.sh", "w") as f:
    f.write("#!/usr/bin/env bash\n")
    f.write("./chall\n")
