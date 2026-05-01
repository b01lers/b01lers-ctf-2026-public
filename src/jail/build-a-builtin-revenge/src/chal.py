#!/usr/local/bin/python3
import builtins

code = input("code > ")

if "." in code:
    print("Nuh uh")
    exit(1)

if "import" in code or "match" in code:
    print("Slopadoodledoo")
    exit(1)

def set_builtin(key, val):
    builtins.__dict__[key] = val

exec = exec
builtins.__dict__.clear()
exec(code, {"set_builtin": set_builtin}, {})