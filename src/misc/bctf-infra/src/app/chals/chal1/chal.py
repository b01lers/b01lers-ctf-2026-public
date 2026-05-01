#!/usr/local/bin/python3
import string

allowed_chars = (string.ascii_lowercase + string.punctuation + string.digits).translate(str.maketrans("", "", "eo"))

inp = input("> ")

for c in inp:
    if c not in allowed_chars:
        print(f"Illegal char {c}")
        exit()
exec(inp)
