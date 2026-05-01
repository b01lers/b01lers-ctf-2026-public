The code for this challenge is short and simple:
```c
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>

#define MAIN ((char *)main)
#define CHALL ((char *)chall)
#define LEN (MAIN - CHALL)

int main(void);

void chall(void) {
    char c = getchar();
    unsigned char i = getchar();
    if (i < LEN) {
        CHALL[i] = c;
    }
}

int main(void) {
    setbuf(stdin, NULL);
    setbuf(stdout, NULL);
    setbuf(stderr, NULL);

    mprotect((char *)((long)CHALL & ~0xfff), 0x1000, PROT_READ | PROT_WRITE | PROT_EXEC);

    chall();
    return 0;
}
```

First, we add the write permission to the memory containing the program's code using `mprotect()`. Then, the program overwrites one byte of the machine code inside the `chall()` function and returns. Note that the `main()` function immediately follows the `chall()` function in the binary.

So, which byte to overwrite, and to what? Just a single byte change isn't going to let us pop a shell. The first order of business should be turning a single byte write into many bytes. We can do that by changing the `ret` instruction at the end of `chall()` to a `nop` (`0x90`). Since the `main()` function *immediately* follows the `chall()` function, this means the CPU just keeps going directly into the `main()` function. That calls `chall()` again, which goes back into `main()`, and so on, for an infinite loop.

Now that we can write as much code as we want, are we done? Not quite. We can't just write shellcode into the binary because it would have to replace something in the `chall()` function, and since every instruction runs on each loop, we can't just replace the code out from under our feet without the program crashing. We need a pocket of code that isn't getting executed. The end of the `main()` function after the call to `chall()` looks like a good spot, but the bounds check prevents us from writing there.

The next step is to disable the bounds check. In x64 assembly, jump instructions refer to labels, which are named offsets into the code to jump to. When the code is assembled, that label gets turned into an offset in bytes from the jump instruction. The `if (i < LEN) {}` block has a `jnl` instruction with the bytes `7d 14`. The `14` means to jump 14 bytes forward if the condition is met. If we replace this number with 0, it just jumps to the following instruction instead, executing the if clause no matter what.

We can now overwrite code in the `main()` function, but as the index to write is an unsigned byte, we can only write 256 bytes ahead. That's still enough space to fit in our shellcode. After it's written, we can just change the `nop` back to a `ret` (`0xc3`) so `chall()` returns, directly to the spot where we wrote the shellcode.
