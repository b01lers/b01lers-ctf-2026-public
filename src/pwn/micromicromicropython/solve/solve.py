from pwn import *

context.log_level = "DEBUG"

with open("win.py", "r") as f:
    code = list(map(str.rstrip, f.readlines()))

p = remote("localhost", 5000)
p.recvuntil(b"paste mode\n")

# # print(code)
for line in code:
    p.recv(4)
    p.sendline(line)

p.interactive()