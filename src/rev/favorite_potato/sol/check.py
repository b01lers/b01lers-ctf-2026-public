# verify that the outputs really correspond to the inputs


from pwn import process
import sys


#DBG = True
DBG = False

NCHUNKS = 100
CODE_DIR = "code_chunks"

def parseRegs(regline):
  pc,A,X,Y,SP,flags = regline.split(' ')
  return [A,X,Y,SP,flags]


def check(idx, startAXY, endAXY):

  A0,X0,Y0 = [f"{v:02x}"  for v in startAXY]
  A,X,Y = [f"{v:02x}"  for v in endAXY]

  print(f"#[{idx}] EMULATOR - input: {A0},{X0},{Y0}, expected_output: {A},{X},{Y}")

  r2 = process("py65mon")
  #print(f"[{idx}]: PID={r2.pid}")
  #if pids:  pids.put(r2.pid)

  out = r2.recvuntil(b"6502:")
  if DBG:   print(f"#[{idx}] DBG - out={out}")
  r2.sendline(f"r a={A0} x={X0} y={Y0}".encode())
  out = r2.recvuntil(b"6502:")
  if DBG:   print(f"#[{idx}] DBG - out={out}")
  startRegs = parseRegs( r2.recvline().strip().decode() )
  if DBG:   print(f"[{idx}]: startRegs={startRegs}")

  for i in range(NCHUNKS):
    r2.sendline(f"load {CODE_DIR}/code_chunk{i}.bin 600".encode())
    out = r2.recvuntil(b"6502:")
    if DBG: print(f"[{idx}]: i={i} out={out}")
    r2.sendline(b"r pc=600")
    r2.recvuntil(b"6502:")
    r2.sendline(b"return")
    r2.recvuntil(b"6502:")
    print(".", end ="", flush = True)

  endRegs = parseRegs( r2.recvline().strip().decode() )
  r2.close()
  print(f"\n[{idx}] START A/X/Y/SP/flags: {'/'.join(startRegs)}")
  print(f"[{idx}] END A/X/Y/SP/flags: {'/'.join(endRegs)}")

  return endRegs[:3] == [A,X,Y]





idx,A0,X0,Y0,A,X,Y = [ int(v)  for v in sys.argv[1:8] ]

if check(idx, (A0,X0,Y0), (A,X,Y)):  exit(0)

exit(1)


