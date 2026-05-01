#!/usr/local/bin/python3
import string

allowed_chars = string.whitespace

inp = input("> ")

for c in inp:
    if c not in allowed_chars:
        print(f"Illegal char {c}")
        exit()

exec(inp)
