# THIS IS THE SAME AS THE REVENGE CHALLENGE SOLVE
from pwn import *

with open("win.py", "rb") as f:
    code = f.read()

code = code.replace(b"\n", b"\r")

p = remote("localhost", 1337)

p.sendlineafter(b"> ", code)
time.sleep(1)
p.sendline(b"cat /flag*; echo")

flag = p.recvline().strip().decode()

print(f"{flag = }")
p.close()