# pickelang

## background

pickelang is an esolang that uses the stack-based pickle virtual machine for evaluation

turing completeness is achieved by recursive pickle functionality
using the `BINPERSID` opcode, `add`, and `getitem` functions

## input

the input will be provided in the format `[A, B, C]` (as a string, NOT a list) and can be retrieved using the `input` function

$\left\{ A, B, C \right\} \subset \mathbb{Z}$

$2 \leq A < 2^{15}$

$1 \leq B < 2^{15}$

$2^{15} \leq C < 2^{16}$

Example `input()` return string value: `[411, 5202, 34891]`

Example expected "return" value from outermost unpickler: `24273`

## output

you must output $\left(A^B\bmod C\right)$. this can be done by stopping outermost unpickler evaluation with
$\left(A^B\bmod C\right)$ on top of the stack.

## additional notes

when submitting your payload, there will be 5 test cases.
there is a 240s timeout, and a 100MB memory limit for each test case.

`struct.pack` and `struct.unpack` are given for convenience

the passing submissions are graded by length of pickle payload, with shorter payloads winning. ties
are broken on execution time.

## the files

[click to download handout zip](insert-handout-link-here)

