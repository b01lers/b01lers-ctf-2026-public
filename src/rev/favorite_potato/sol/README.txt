Favorite Potato (by dm)
=======================

Well, this turned out to be our easiest rev :P TBH, I thought that this would be harder, given the ancient 
8-bit architecture - I wonder if it was too AI friendly or just a good value for flag trades. Whatever... it 
gives me a license to post more complex C64 codes next year ;)

The flow of the challenge was simple: it generates three random values (A,X,Y), feeds those to a Motorola 6502 
binary, you get back a new triplet (A,X,Y) which gets printed. Your job is to recover the original A,X,Y. The 
set of different states is fairly small, 3 x 8 = 24 bits (16M or so), so I demanded 20 successful rounds -> 
that way your inverse has to work for an O(1) fraction of all outputs.

You got a simple 9-byte test binary which did

  A += 0x2a
  X += 1
  Y -= 2

to illustrate how things work. The real challenge binary was far longer; it was doing 10k rounds of 
add-rotate-xor (ARX) transformations on A,X,Y, with random constants (k1-k5, r1-r3) in each round:

  #one ARX step
  A = A + X + k1
  X = ror(X, r1)
  Y ^= X ^ k2
  Y = Y + A + k3
  A = ror(A, r2)
  X ^= A ^ k4
  Y = ror(Y, r3)
  A ^= Y ^ k5

I chose ARX because it is invertible. The loop over 10k was completely rolled out, so there was a lot of 
repetitive code. There were a couple hints about that - you got a gzipped binary to show you that the code 
compressed well, and the number of rounds was a nice big power of 10 to yield a telltale binary size of 5820001 
(+1 for the final RTS). This is, of course, much longer code than the nearly 64kB you can load onto a normal 
C64 - so the challenge had you imagine some "upgraded" hardware that can run arbitrary long codes.

The M6502 has a very limited set of instructions, for example, it has no direct math on X and Y besides 
INC/DEC. Normally one just moves values between A and X,Y via direct stores/loads to/from RAM but I wanted code 
that has no reference to any specific address, so I implemented ARX using 9 primitives:

  swaps:  A<->X, A<->Y, X<->Y
  adds:   A+=k, X+=A, Y+=A
  rors:   A=ror(A,r1)
  xors:   A^=k, A^=Y

each of which only used the stack. To make your life simpler, these all started on PHP and ended on PLP (I did 
optimize out a few redundant swaps from the ARX step on the level of primitives but there were zero further 
optimization on ASM level). The final binary for one ARX step was just 582 bytes - I hoped that you would 
reverse that much from the beginning of the binary and then realize that the rest just repeats with different 
constants.

---

The usual tools (e.g., disassembler da65 or ghidra) impose the architecture limit of 64kB and truncate the 
binary - but that is not that big a hurdle. One can just break the binary up into smaller pieces and run the 
tools on those. Once you have the full code reversed in terms of either ARX steps, or the primitives, it is 
straightforward to write an inverse the transformation in, e.g., python and solve the challenge. The attached 
solvers (sol.py, sol2.py) do just that. I also included a variant (sol+check.py) that runs the original binary 
via the py65 emulator (broken into 100 chunks) to verify that the inputs indeed reproduce the outputs. This 
particular emulator is super slow but it does demonstrate correctness.

Hope you had fun with it, see you next year,
--dm
