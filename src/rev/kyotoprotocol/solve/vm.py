import collections
import json

data = json.loads(open("parsed.json").read())

start = 9823
pc = start

exports = collections.defaultdict(int)
args = []

input_data = ""


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
            return "?"


def eval_expression(expr):
    match expr["type"]:
        case "number":
            return expr["value"]
        case "binary_op":
            left = eval_expression(expr["left"])
            right = eval_expression(expr["right"])
            match expr["operation"]:
                case "+":
                    return left + right
                case "-":
                    return left - right
                case "*":
                    return left * right
                case "/":
                    return left // right
                case "%":
                    return left % right
                case _:
                    raise Exception(f"Unknown binary operation: {expr['operation']}")
        case "env_str":
            raise Exception("env_str not implemented")
        case "env":
            return exports[expr["name"]]
        case "arg":
            if expr["position"] >= len(args):
                return 0
            return args[expr["position"]]
        case _:
            raise Exception(f"Unknown expression type: {expr['type']}")


did_move = True

while did_move:
    # print(f"PC: {pc}, Args: {args}")
    operations = data[str(pc)]
    did_move = False
    old_pc = pc
    old_args = args[:]
    should_read = False
    for operation in operations:
        op_debug_str = None
        op_debug_args = None
        match operation["type"]:
            case "export":
                value = eval_expression(operation["value"])
                assert isinstance(value, int)
                exports[operation["name"]] = value
                if operation["value"]["type"] != "number":
                    op_debug_str = f"EXPORT: {operation['name']} = {value} ; {print_expr(operation['value'])}"
                else:
                    op_debug_str = f"EXPORT: {operation['name']} = {value}"
            case "next":
                pc = operation["next"]
                did_move = True
                args = [eval_expression(arg) for arg in operation["args"]]
                op_debug_str = f"NEXT: -> {pc}"
                op_debug_args = operation["args"]
            case "write":
                op_debug_str = f"WRITE: {operation['line']}"
            case "read":
                op_debug_str = "READ:"
                should_read = True
            case "input":
                for i in range(len(input_data)):
                    exports[f"i{i}"] = ord(input_data[i])
            case "branch":
                left = eval_expression(operation["condition"]["left"])
                right = eval_expression(operation["condition"]["right"])
                result = None
                match operation["condition"]["operation"]:
                    case "==":
                        result = left == right
                    case "!=":
                        result = left != right
                    case "<":
                        result = left < right
                    case "<=":
                        result = left <= right
                    case ">":
                        result = left > right
                    case ">=":
                        result = left >= right
                    case _:
                        raise Exception(f"Unknown branch operation: {operation['condition']['operation']}")
                target = operation["true_target"] if result else operation["false_target"]
                pc = target["next"]
                args = [eval_expression(arg) for arg in target["args"]]
                did_move = True
                op_debug_str = f"JUMP ({print_expr(operation['condition']['left'])} {operation['condition']['operation']} {print_expr(operation['condition']['right'])}) ? {'->' if result else ''}{operation['true_target']['next']} : {'->' if not result else ''}{operation['false_target']['next']}"
                op_debug_args = target["args"]
            case "check":

                def check():
                    if exports["g_8694"] != 1820085546:
                        print(f"c1: {exports['g_8694']} != 1820085546")
                        return False
                    if exports["g_4968"] != 1410707190:
                        print(f"c2: {exports['g_4968']} != 1410707190")
                        return False
                    if exports["g_2431"] != 972076578:
                        print(f"c3: {exports['g_2431']} != 972076578")
                        return False
                    if exports["g_3694"] != 1718772620:
                        print(f"c4: {exports['g_3694']} != 1718772620")
                        return False
                    if exports["g_7965"] != 333333333:
                        print(f"c5: {exports['g_7965']} != 333333333")
                        return False
                    if exports["g_1829"] != 333333333:
                        print(f"c6: {exports['g_1829']} != 333333333")
                        return False
                    if exports["g_2184"] != 333333333:
                        print(f"c7: {exports['g_2184']} != 333333333")
                        return False
                    return True

                target = operation["correct"] if check() else operation["incorrect"]
                pc = target["next"]
                args = [eval_expression(arg) for arg in target["args"]]
                did_move = True
            case "decrypt":
                # decrypt logic, doesn't matter if we win
                op_debug_str = "DECRYPT"
                pc = operation["next"]
                args = []
                did_move = True
            case _:
                print(f"Unknown operation type: {operation['type']}")
                exit(0)
        if op_debug_str:
            if op_debug_args:
                debug_args = []
                for i, arg in enumerate(op_debug_args):
                    if arg["type"] == "arg" and arg["position"] == i:
                        debug_args.append(".")
                        continue
                    debug_args.append(str(print_expr(arg)))
                debug_args = " ".join(debug_args)
                print(
                    f"{old_pc:04d} | {' '.join([str(arg) for arg in old_args]):<20}| {op_debug_str:<60} | {debug_args}"
                )
            else:
                print(f"{old_pc:04d} | {' '.join([str(arg) for arg in old_args]):<20}| {op_debug_str}")
        if should_read:
            input_data = input(">")
            should_read = False
