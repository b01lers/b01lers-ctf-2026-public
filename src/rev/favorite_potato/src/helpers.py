import gzip

# pre-extract ARX constants from code.bin

code1 = open("code1.bin", "rb").read()[:-1]
N1 = len(code1)
binary = gzip.open("code.bin.gz", "rb").read()[:-1]
Nblocks = len(binary) // N1
assert Nblocks * N1 == len(binary)

locs1 = [pos  for pos in range(N1)   if code1[pos] == 0x87]

constants = []
for i in range(Nblocks):
  keys = [binary[k + i * N1]   for k in locs1]
  kvals = [keys[k]  for k in (0,2,3,5,7)]
  rvals = [keys[k] for k in (1,4,6)]
  constants.append((kvals, rvals))

# one ARX step

def ror(v, k):  return (v >> k) + ((v << (8 - k)) & 0xff)

def ARX(regs, kvals, rvals):
  A,X,Y = regs
  k1,k2,k3,k4,k5 = kvals
  r1,r2,r3 = rvals
  # ops 1-3
  A = (A + X + k1) & 0xff
  X = ror(X, r1 & 7)
  Y ^= X ^ k2
  # ops 4-6
  Y = (Y + A + k3) & 0xff
  A = ror(A, r2 & 7)
  X ^= A ^ k4
  #ops 7-8
  Y = ror(Y, r3 & 7)
  A ^= Y ^ k5
  return A,X,Y


# simulate effect of binaries

def run_c64(fn, A, X, Y):
  if fn == "test.bin":
    return (A + 0x2a) & 0xff, (X - 1) & 0xff, (Y + 2) & 0xff

  elif fn == "code.bin":
    for kvals,rvals in constants:
      A, X, Y = ARX((A,X,Y), kvals, rvals)
    return A, X, Y

  else:
    return -1, -1, -1
