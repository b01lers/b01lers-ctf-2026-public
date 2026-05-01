# like sol.py but includes a check that runs the binary in a real c64 emulator (->install py65)
# to verify that the outputs really correspond to the inputs


import subprocess
import gzip
import os
import sys

from pwn import remote
from concurrent.futures import ProcessPoolExecutor, wait
from sol import solve


#DBG = True
DBG = False

NCHUNKS = 100
CODE_DIR = "code_chunks"

def createCodeChunks():
  binary = gzip.open("code.bin.gz", "rb").read()
  # break binary into 100 chunks (each chunk contains 100 ARX steps)
  os.system(f"mkdir {CODE_DIR}")
  CHUNKSIZE = 582 * 100
  for i in range(NCHUNKS):
    open(f"{CODE_DIR}/code_chunk{i}.bin", "wb").write(binary[i*CHUNKSIZE:(i+1)*CHUNKSIZE] + b"\x60")


def runCheck(idx, startAVX, endAVX):
  startArgs = [str(v)  for v in startAVX]
  endArgs = [str(v)  for v in endAVX]
  result = subprocess.run(["python3", "check.py", f"{idx}", *startArgs, *endArgs])
  good = result.returncode == 0
  if good:
    print(f"[{idx}] => MATCHES!")
  else:
    print(f"[{idx}] => wrong :/")
  return good


if __name__ == "__main__":

  #doCheck = len(sys.argv) > 1
  doCheck = True
  createCodeChunks()

  r = remote("localhost", 1337)
  r.sendline(b"R")
  r.recvuntil(b"Final output")

  executor = ProcessPoolExecutor(max_workers=5)
  cleanup = False

  try:
    solves = []
    futures = []
    for i in range(20):
      ans = r.recvline().strip().split()[-3:]
      ans = [ int(v.split(b"=")[1])  for v in ans]
      solution = solve(ans)
      # check solution in emulator, if requested
      if doCheck:
        futures.append( executor.submit(runCheck, i, solution, ans) )
        #runCheck(i, solution, ans)
      print(i, ans, solution)
      solves.append(solution)

    for A,X,Y in solves:
      r.recvuntil(b"Input #")
      r.sendline(f"{A},{X},{Y}".encode())
    print(r.recvall())
    r.close()

    # wait for all checks to finish
    print(f"CHECK results: { [ f.result()  for f in futures] }")

  except KeyboardInterrupt:
    print("CTRL+C pressed, quitting")
    cleanup = True
  except Exception as e:
    print(f"Quitting on exception: {e}")
    cleanup = True

  if cleanup:
    os.killpg(os.getpgid(0), 9)  #kill everything in our process group (kills all check subprocesses)
