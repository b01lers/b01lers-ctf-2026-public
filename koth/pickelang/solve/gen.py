import enum as enum2
from ast import NodeVisitor
import ast
from pickle import *
from pickletools import dis
import sys
if sys.version_info >= (3, 14):
    from compression import zstd
else:
    from backports import zstd


_simple_enum = enum2._simple_enum
_Precedence = ast._Precedence


def dump(v, **kwargs):
    return (ast.dump(v, **kwargs) if not isinstance(v, list) else
            '[' + ', '.join([ast.dump(v2, **kwargs) for i, v2 in enumerate(v)]) + ']')


def iter_fields(node):
    """
    Yield a tuple of ``(fieldname, value)`` for each field in ``node._fields``
    that is present on *node*.
    """
    for field in node._fields:
        try:
            yield field, getattr(node, field)
        except AttributeError:
            pass


def iter_child_nodes(node):
    """
    Yield all direct child nodes of *node*, that is, all fields that are nodes
    and all items of fields that are lists of nodes.
    """
    for name, field in iter_fields(node):
        if isinstance(field, ast.AST):
            yield field
        elif isinstance(field, list):
            for item in field:
                if isinstance(item, ast.AST):
                    yield item


def _splitlines_no_ff(source):
    """Split a string into lines ignoring form feed and other chars.

    This mimics how the Python parser splits source code.
    """
    idx = 0
    lines = []
    next_line = ''
    while idx < len(source):
        c = source[idx]
        next_line += c
        idx += 1
        # Keep \r\n together
        if c == '\r' and idx < len(source) and source[idx] == '\n':
            next_line += '\n'
            idx += 1
        if c in '\r\n':
            lines.append(next_line)
            next_line = ''

    if next_line:
        lines.append(next_line)
    return lines


def _pad_whitespace(source):
    r"""Replace all chars except '\f\t' in a line with spaces."""
    result = ''
    for c in source:
        if c in '\f\t':
            result += c
        else:
            result += ' '
    return result


def get_source_segment(source, node, *, padded=False):
    """Get source code segment of the *source* that generated *node*.

    If some location information (`lineno`, `end_lineno`, `col_offset`,
    or `end_col_offset`) is missing, return None.

    If *padded* is `True`, the first line of a multi-line statement will
    be padded with spaces to match its original position.
    """
    try:
        if node.end_lineno is None or node.end_col_offset is None:
            return None
        lineno = node.lineno - 1
        end_lineno = node.end_lineno - 1
        col_offset = node.col_offset
        end_col_offset = node.end_col_offset
    except AttributeError:
        return None

    lines = _splitlines_no_ff(source)
    if end_lineno == lineno:
        return lines[lineno].encode()[col_offset:end_col_offset].decode()

    if padded:
        padding = _pad_whitespace(lines[lineno].encode()[:col_offset].decode())
    else:
        padding = ''

    first = padding + lines[lineno].encode()[col_offset:].decode()
    last = lines[end_lineno].encode()[:end_col_offset].decode()
    lines = lines[lineno + 1:end_lineno]

    lines.insert(0, first)
    lines.append(last)
    return ''.join(lines)


def walk(node):
    """
    Recursively yield all descendant nodes in the tree starting at *node*
    (including *node* itself), in no specified order.  This is useful if you
    only want to modify nodes in place and don't care about the context.
    """
    from collections import deque
    todo = deque([node])
    while todo:
        node = todo.popleft()
        todo.extend(iter_child_nodes(node))
        yield node


_SINGLE_QUOTES = ("'", '"')
_MULTI_QUOTES = ('"""', "'''")
_ALL_QUOTES = (*_SINGLE_QUOTES, *_MULTI_QUOTES)


name_incrementer = 10
name_map = {}


ARGS_MEMO_BASE = 50000


class FunctionUnparser(NodeVisitor):
    def __init__(self, *, _avoid_backslashes=False):
        self.sources = []

    def pickle_push_get(self, v: int):
        assert isinstance(v, int)
        self.write(GET + str(v).encode() + b'\n')

    def pickle_put(self, v: int):
        assert isinstance(v, int)
        self.write(PUT + str(v).encode() + b'\n')

    def pickle_push_int(self, v: int):
        assert isinstance(v, int)
        self.write(INT + str(v).encode() + b'\n')

    def pickle_push_str(self, v: str):
        assert isinstance(v, str)
        assert len(v) < 2 ** 32
        self.write(BINSTRING + len(v).to_bytes(4, 'little') + v.encode())

    def pickle_push_binbytes(self, v):
        assert isinstance(v, bytes)
        assert len(v) < 2 ** 32
        self.write(BINBYTES + len(v).to_bytes(4, 'little') + v)

    def write(self, *text: bytes):
        """Add new source parts"""
        self.sources.extend(text)

    # ========== visiting stuff below ==========

    def traverse(self, node):
        if isinstance(node, list):
            for item in node:
                self.traverse(item)
        else:
            super().visit(node)

    # Note: as visit() resets the output text, do NOT rely on
    # NodeVisitor.generic_visit to handle any nodes (as it calls back in to
    # the subclass visit() method, which resets self._source to an empty list)
    def visit(self, node):
        self.sources = []
        self.traverse(node)
        return b"".join(self.sources)

    def visit_Module(self, node):
        raise NotImplementedError("todo impl this")

    def visit_FunctionType(self, node):
        raise NotImplementedError('todo impl this')

    def visit_Expr(self, node):
        # https://docs.python.org/3.11/library/ast.html#ast.Expr
        # "when an expression, such as a function call, appears as a statement by itself
        #  with its return value not used or stored, it is wrapped in this container"
        self.traverse(node.value)
        self.write(POP)

    def visit_NamedExpr(self, node):
        raise NotImplementedError('todo impl this')

    def visit_Import(self, node):
        raise NotImplementedError('todo impl this')

    def visit_ImportFrom(self, node):
        raise NotImplementedError('todo impl this')

    def visit_Assign(self, node):
        global name_incrementer
        assert isinstance(node.targets, list)
        assert len(node.targets) == 1, "no more than 1 target per assign"
        target = node.targets[0]
        if isinstance(target, ast.Name):
            name: str = target.id
            assert name not in name_map, f"{name} already seen"
            name_map[name] = name_incrementer
            name_incrementer += 1
            self.traverse(node.value)
            self.pickle_put(name_map[name])
            self.write(POP)
        elif isinstance(target, ast.Subscript):
            indices = []
            while isinstance(target, ast.Subscript):
                indices.insert(0, target.slice)
                target = target.value
            for _ in indices[:-1]:
                self.write(GLOBAL + b'.\ngetitem\n')
            self.traverse(target)
            for index in indices[:-1]:
                self.traverse(index)
                self.write(TUPLE2 + REDUCE)
            self.traverse(indices[-1])
            self.traverse(node.value)
            self.write(SETITEM + POP)
        else:
            raise NotImplementedError("todo impl this")


    def visit_AugAssign(self, node):
        assert node.op.__class__ == ast.Add, f"bad binop {node.op.__class__}"
        target = node.target
        if isinstance(target, ast.Name):
            self.write(GLOBAL + b'.\nadd\n')
            self.traverse(node.target)
            self.traverse(node.value)
            self.write(TUPLE2)
            self.write(REDUCE)
            self.pickle_put(name_map[node.target.id])
            self.write(POP)
        else:
            raise NotImplementedError('todo impl this')

    def visit_AnnAssign(self, node):
        raise NotImplementedError('todo impl this')

    def visit_Return(self, node):
        self.traverse(node.value)
        self.write(STOP)

    def visit_Pass(self, node):
        pass

    def visit_Break(self, node):
        raise NotImplementedError('todo impl this')

    def visit_Continue(self, node):
        raise NotImplementedError('todo impl this')

    def visit_Delete(self, node):
        raise NotImplementedError('todo impl this')

    def visit_Assert(self, node):
        raise NotImplementedError('todo impl this')

    def visit_Global(self, node):
        pass

    def visit_Nonlocal(self, node):
        raise NotImplementedError('todo impl this')

    def visit_Await(self, node):
        raise NotImplementedError('todo impl this')

    def visit_Yield(self, node):
        raise NotImplementedError('todo impl this')

    def visit_YieldFrom(self, node):
        raise NotImplementedError('todo impl this')

    def visit_Raise(self, node):
        raise NotImplementedError('todo impl this')

    def visit_Try(self, node):
        raise NotImplementedError('todo impl this')

    def visit_TryStar(self, node):
        raise NotImplementedError('todo impl this')

    def visit_ExceptHandler(self, node):
        raise NotImplementedError('todo impl this')

    def visit_ClassDef(self, node):
        raise NotImplementedError('todo impl this')

    def visit_FunctionDef(self, node):
        global name_incrementer
        assert len(node.args.posonlyargs) == 0
        assert len(node.args.kwonlyargs) == 0
        assert len(node.args.kw_defaults) == 0
        assert len(node.args.defaults) == 0
        self.write(PROTO + b'\x05')
        for i, arg in enumerate(node.args.args):
            self.pickle_push_get(ARGS_MEMO_BASE + i)
            assert arg.arg not in name_map, f"{arg.arg} varname already seen"
            name_map[arg.arg] = name_incrementer
            self.pickle_put(name_incrementer)
            name_incrementer += 1
            self.write(POP)
        for stmt in node.body:
            self.traverse(stmt)

    def visit_AsyncFunctionDef(self, node):
        raise NotImplementedError('todo impl this')

    def visit_For(self, node):
        raise NotImplementedError('todo impl this')

    def visit_AsyncFor(self, node):
        raise NotImplementedError('todo impl this')

    def visit_If(self, node):
        raise NotImplementedError('todo impl this')

    def visit_While(self, node):
        raise NotImplementedError('todo impl this')

    def visit_With(self, node):
        raise NotImplementedError('todo impl this')

    def visit_AsyncWith(self, node):
        raise NotImplementedError('todo impl this')

    def visit_JoinedStr(self, node):
        raise NotImplementedError('todo impl this')

    def visit_FormattedValue(self, node):
        raise NotImplementedError('todo impl this')

    def visit_Name(self, node):
        assert node.id in name_map, f"{node.id} not defined"
        self.pickle_push_get(name_map[node.id])

    def visit_Constant(self, node):
        v = node.value
        if isinstance(v, int):
            self.pickle_push_int(v)
            return
        if isinstance(v, str):
            self.pickle_push_str(v)
            return
        if isinstance(v, bytes):
            self.pickle_push_binbytes(v)
            return
        raise NotImplementedError('todo impl this')

    def visit_List(self, node):
        self.write(MARK)
        for elt in node.elts:
            self.traverse(elt)
        self.write(LIST)

    def visit_ListComp(self, node):
        raise NotImplementedError('todo impl this')

    def visit_GeneratorExp(self, node):
        raise NotImplementedError('todo impl this')

    def visit_SetComp(self, node):
        raise NotImplementedError('todo impl this')

    def visit_DictComp(self, node):
        raise NotImplementedError('todo impl this')

    def visit_comprehension(self, node):
        raise NotImplementedError('todo impl this')

    def visit_IfExp(self, node):
        raise NotImplementedError('todo impl this')

    def visit_Set(self, node):
        raise NotImplementedError('todo impl this')

    def visit_Dict(self, node):
        self.write(MARK)
        for k, v in zip(node.keys, node.values):
            self.traverse(k)
            self.traverse(v)
        self.write(DICT)

    def visit_Tuple(self, node):
        self.write(MARK)
        for elt in node.elts:
            self.traverse(elt)
        self.write(TUPLE)

    def visit_UnaryOp(self, node):
        assert isinstance(node.operand, ast.Constant)
        assert isinstance(node.operand.value, int)
        self.pickle_push_int(-node.operand.value)

    def visit_BinOp(self, node):
        assert node.op.__class__ == ast.Add, f"bad binop {node.op.__class__}"
        self.write(GLOBAL + b'.\nadd\n')
        self.traverse(node.left)
        self.traverse(node.right)
        self.write(TUPLE2)
        self.write(REDUCE)

    def visit_Compare(self, node):
        raise NotImplementedError('todo impl this')

    def visit_BoolOp(self, node):
        raise NotImplementedError('todo impl this')

    def visit_Attribute(self, node):
        raise NotImplementedError('todo impl this')

    def visit_Call(self, node):
        if len(node.keywords) > 0:
            raise NotImplementedError('todo impl this')
        if isinstance(node.func, ast.Name) and node.func.id == "input":
            self.write(GLOBAL + b'.\ninput\n' + EMPTY_TUPLE + REDUCE)
            return
        if isinstance(node.func, ast.Name) and node.func.id == "print":
            self.write(GLOBAL + b'.\nprint\n' + MARK)
            for arg in node.args:
                self.traverse(arg)
            self.write(TUPLE + REDUCE)
            return
        self.traverse(node.func)
        for i, arg in enumerate(node.args):
            self.traverse(arg)
            self.pickle_put(ARGS_MEMO_BASE + i)
            self.write(POP)
        self.write(BINPERSID)

    def visit_Subscript(self, node):
        self.write(GLOBAL + b'.\ngetitem\n')
        self.traverse(node.value)
        self.traverse(node.slice)
        self.write(TUPLE2 + REDUCE)

    def visit_Starred(self, node):
        raise NotImplementedError('todo impl this')

    def visit_Ellipsis(self, node):
        raise NotImplementedError('todo impl this')

    def visit_Slice(self, node):
        raise NotImplementedError('todo impl this')

    def visit_Match(self, node):
        raise NotImplementedError('todo impl this')

    def visit_arg(self, node):
        raise NotImplementedError('todo impl this')

    def visit_arguments(self, node):
        raise NotImplementedError('todo impl this')

    def visit_keyword(self, node):
        raise NotImplementedError('todo impl this')

    def visit_Lambda(self, node):
        raise NotImplementedError('todo impl this')

    def visit_alias(self, node):
        raise NotImplementedError('todo impl this')

    def visit_withitem(self, node):
        raise NotImplementedError('todo impl this')

    def visit_match_case(self, node):
        raise NotImplementedError('todo impl this')

    def visit_MatchValue(self, node):
        raise NotImplementedError('todo impl this')

    def visit_MatchSingleton(self, node):
        raise NotImplementedError('todo impl this')

    def visit_MatchSequence(self, node):
        raise NotImplementedError('todo impl this')

    def visit_MatchStar(self, node):
        raise NotImplementedError('todo impl this')

    def visit_MatchMapping(self, node):
        raise NotImplementedError('todo impl this')

    def visit_MatchClass(self, node):
        raise NotImplementedError('todo impl this')

    def visit_MatchAs(self, node):
        raise NotImplementedError('todo impl this')

    def visit_MatchOr(self, node):
        raise NotImplementedError("todo impl this")


def main():
    global name_incrementer
    tree = ast.parse(open("code.py").read())
    functions: list[ast.stmt] = tree.body
    assert all(isinstance(fn, ast.FunctionDef) for fn in functions)
    functions: list[ast.FunctionDef]
    for i, fn in enumerate(functions):
        name_map[fn.name] = name_incrementer
        name_incrementer += 1
    total_payload = b""
    for fn in functions:
        # print(dump(fn, indent=4))
        func_body = FunctionUnparser().visit(fn)
        total_payload += BINBYTES + len(func_body).to_bytes(4, 'little') + func_body
        total_payload += PUT + str(name_map[fn.name]).encode() + b'\n'
    total_payload += GET + str(name_map["main"]).encode() + b'\n' + BINPERSID + STOP

    # jank asf
    with open("out.pkl.zstd", 'wb') as f:
        f.write(zstd.compress(total_payload))
    real_input = __builtins__.input
    __builtins__.input = lambda *a: total_payload.hex() if len(a) and "pickelang" in a[0] else real_input()
    import pickelang


if __name__ == '__main__':
    main()
