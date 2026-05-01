This challenge is very simple, you are provided this script and its output:

```python
import base64

from Crypto.Cipher import AES
from Crypto.Util.strxor import strxor

flag = open("flag.txt").read().strip()

s = ""
for c in flag:
    s += c * 2
flag = s

cipher = AES.new(b"lasagna!" * 2, AES.MODE_ECB)
result = b"\0" * len(flag)

for i in range(len(result)):
    ciphertext = cipher.encrypt(flag[i].encode() * 16)
    layer = b"\0" * i + ciphertext
    if len(layer) < len(result):
        layer += b"\0" * (len(result) - len(layer))
    if len(layer) > len(result):
        layer = layer[len(result):] + layer[len(layer)-len(result):len(result)]
    result = strxor(result, layer)

print(base64.b64encode(result).decode())
```

There are many unconventional things going on here. First, the AES key is hardcoded as the string "lasagna!" repeated twice. Second, it will only ever encrypt plaintext that is the same character of the flag repeated 16 times, so there are only 256 options for plaintext. Third, each character in the flag is repeated twice, so "abcd" would become "aabbccdd" for example.

Those ciphertexts are all XORed together in some weird way to get the result. The process is hard to describe, so here is a visualization. Let's assume there are 8 characters in the flag, and ciphertext blocks are only 4 bytes long.

```
Ciphertext 1: 1234
Ciphertext 2: 5678
Ciphertext 3: 9ABC
Ciphertext 4: DEF0
Ciphertext 5: F012
Ciphertext 6: 3456
Ciphertext 7: 789A
Ciphertext 8: BCDE

Result:

1234
 5678
  9ABC
   DEF0
    F012
6    345
9A    78
CDE    B
----------- (XOR all layers together)
WXYZWXYZ <- in output.txt
```

The result is set to the first block of ciphertext. Then, the second block is XORed to it, but the block is shifted over by one byte first. The third is shifted by two bytes, and so on. When blocks reach the end, they wrap around to the beginning. For 16-byte ciphertext blocks, this means each byte of the result is 16 bytes of ciphertext XORed together at different offsets. There is seemingly no way to extract individual bytes from this combination, since there are any number of ways the bytes could all XOR to the given value.

However, there is one thing you know: the first five characters of the plaintext are `bctf{`. Actually, if you move the last character to the start, you know the first six characters are `}bctf{`. Every character is repeated, so with this you know the first twelve characters of the flag that is passed to the scrambler.

Since the key is given, you can determine the ciphertext for any character. Let's assume the layer structure looks something like this:

```
AAAAAAAAAAAAAAAAIIIIIIIIIIIIIIII
IAAAAAAAAAAAAAAAAIIIIIIIIIIIIIII
JJBBBBBBBBBBBBBBBBJJJJJJJJJJJJJJ
JJJBBBBBBBBBBBBBBBBJJJJJJJJJJJJJ
KKKKCCCCCCCCCCCCCCCCKKKKKKKKKKKK
KKKKKCCCCCCCCCCCCCCCCKKKKKKKKKKK
LLLLLLDDDDDDDDDDDDDDDDLLLLLLLLLL
LLLLLLLDDDDDDDDDDDDDDDDLLLLLLLLL
MMMMMMMMEEEEEEEEEEEEEEEEMMMMMMMM
MMMMMMMMMEEEEEEEEEEEEEEEEMMMMMMM
NNNNNNNNNNFFFFFFFFFFFFFFFFNNNNNN
NNNNNNNNNNNFFFFFFFFFFFFFFFFNNNNN
OOOOOOOOOOOOGGGGGGGGGGGGGGGGOOOO
OOOOOOOOOOOOOGGGGGGGGGGGGGGGGOOO
PPPPPPPPPPPPPPHHHHHHHHHHHHHHHHPP
PPPPPPPPPPPPPPPHHHHHHHHHHHHHHHHP
```

By knowing the first twelve ciphertext blocks, you can XOR them back out of the result to get this:

```
                IIIIIIIIIIIIIIII
I                IIIIIIIIIIIIIII
JJ                JJJJJJJJJJJJJJ
JJJ                JJJJJJJJJJJJJ
KKKK                KKKKKKKKKKKK
KKKKK                KKKKKKKKKKK
LLLLLL                LLLLLLLLLL
LLLLLLL                LLLLLLLLL
MMMMMMMM                MMMMMMMM
MMMMMMMMM                MMMMMMM
NNNNNNNNNN                NNNNNN
NNNNNNNNNNN                NNNNN
OOOOOOOOOOOOGGGGGGGGGGGGGGGGOOOO
OOOOOOOOOOOOOGGGGGGGGGGGGGGGGOOO
PPPPPPPPPPPPPPHHHHHHHHHHHHHHHHPP
PPPPPPPPPPPPPPPHHHHHHHHHHHHHHHHP
```

Notice how the 13th byte has only two unknowns to it now. If we can guess the next two characters, we can remove them as well, and if the two characters are correct, the 14th byte will become `0x00` since everything has been removed. There are only 65536 combinations of the next two characters, so we can just try them all.

We can keep a list of each combination of characters to get a null 14th byte. Then, for each one, we try each possible character for the 15th byte, and see which ones have a null 15th byte as well. If they don't, they must be wrong, so we eliminate them. In practice, after a couple rounds this list will only ever contain one or two possibilities, since there is only a 1/256 chance for a random ciphertext to create a null byte at that position. Once we have gone through every character, there should only be one possibility, which is the flag.
