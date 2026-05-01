# this variant only uses the primitive building blocks, no need to understand what their
# combination really means

from pwn import *
import gzip

# invert one step, using the primitives

"""
A += k1
A <-> X
X += A
A = ror(A, r1)
A <-> Y
--
A ^= Y
A ^= k2
X <-> Y
A <-> Y
Y += A
--
A <-> Y
A += k3
A <-> Y
A = ror(A,r2)
A <-> X
--
X <-> Y
A ^= Y
A ^= k4
X <-> Y
A <-> X
--
A <-> Y
A = ror(A, r3)
A <-> Y
A ^= Y
A ^= k5
"""

def rol(v, k):  return ((v << k) & 0xff) + (v >> (8 - k))

def invStep(regs, kvals, rvals):
  A,X,Y = regs
  k1,k2,k3,k4,k5 = kvals
  r1,r2,r3 = rvals
  A^= k5
  A^= Y
  A,Y = Y,A
  A = rol(A, r3 & 7)
  A,Y = Y,A
  #
  A,X = X,A
  X,Y = Y,X
  A^= k4
  A^= Y
  X,Y = Y,X
  #
  A,X = X,A
  A = rol(A, r2 & 7)
  A,Y = Y,A
  A = (A - k3) & 0xff
  A,Y = Y,A
  #
  Y = (Y - A) & 0xff
  A,Y = Y,A
  X,Y = Y,X
  A^= k2
  A^= Y
  #
  A,Y = Y,A
  A = rol(A, r1 & 7)
  X = (X - A) & 0xff
  A,X = X,A
  A = (A - k1) & 0xff
  return A,X,Y


# solve

# get constants from binary
klocs = [3, 167, 232, 396, 580]
rlocs = [38, 263, 451]

code = gzip.open("code.bin.gz", "rb").read()
Nblocks = 10**4
blockSize = (len(code) - 1) // Nblocks
assert (len(code) - 1) % blockSize == 0

constants = []
for i in range(Nblocks):
  kvals = [code[k + i * blockSize] for k in klocs]
  rvals = [code[k + i * blockSize] for k in rlocs]
  constants.append((kvals, rvals))

def solve(regs):
  A,X,Y = regs
  for i in range(Nblocks):
    kvals, rvals = constants[Nblocks - i - 1]
    A,X,Y = invStep((A,X,Y), kvals, rvals)
  return (A,X,Y)

r = remote("localhost", 1337)
r.sendline(b"R")
r.recvuntil(b"Final output")
solves = []
for i in range(20):
  ans = r.recvline().strip().split()[-3:]
  ans = [ int(v.split(b"=")[1])  for v in ans]
  sol = solve(ans)
  print(i, ans, sol)
  solves.append(sol)


for A,X,Y in solves:
  r.recvuntil(b"Input #")
  r.sendline(f"{A},{X},{Y}".encode())

r.interactive()
