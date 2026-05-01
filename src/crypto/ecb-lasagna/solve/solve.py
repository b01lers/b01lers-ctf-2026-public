from pwn import *
import base64

from Crypto.Cipher import AES
from Crypto.Util.strxor import strxor

result = base64.b64decode(read("../dist/output.txt"))

cipher = AES.new(b"lasagna!" * 2, AES.MODE_ECB)
codebook = {}
for b in range(0x20, 0x7f):
    codebook[b] = cipher.encrypt(chr(b).encode() * 16)

def xorblock(base, block, offset):
    layer = b"\0" * offset + block
    if len(layer) < len(result):
        layer += b"\0" * (len(result) - len(layer))
    if len(layer) > len(result):
        layer = layer[len(result):] + layer[len(layer)-len(result):len(result)]
    return strxor(base, layer)

flag_prefix = b"}}bbccttff{{"
for i in range(len(flag_prefix)):
    result = xorblock(result, codebook[flag_prefix[i]], (i - 2) % len(result))

possibilities = []

for c1 in range(0x20, 0x7f):
    for c2 in range(0x20, 0x7f):
       base = result
       base = xorblock(base, codebook[c1], 10)
       base = xorblock(base, codebook[c1], 11)
       base = xorblock(base, codebook[c2], 12)
       base = xorblock(base, codebook[c2], 13)
       if base[13] == 0:
           s = (chr(c1) + chr(c2)).encode()
           #print(s.decode())
           possibilities.append((s, base))

length = 14
while length < len(result) - 2:
    #print(length)
    new_possibilities = []
    for possibility, base in possibilities:
        for c in range(0x20, 0x7f):
            new_base = base
            new_base = xorblock(new_base, codebook[c], length)
            new_base = xorblock(new_base, codebook[c], length + 1)
            if new_base[length] == 0 and new_base[length + 1] == 0:
                s = possibility + chr(c).encode()
                #print(s.decode())
                new_possibilities.append((s, new_base))
    possibilities = new_possibilities
    length += 2

flag, base = possibilities[0]
print("bctf{" + flag.decode() + "}")
