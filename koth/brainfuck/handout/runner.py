#!/usr/bin/env python3
import random
import subprocess
import sys

N = 50
TIMEOUT = 10
RANGES = {"a": (10, 999), "b": (50, 100), "c": (100, 999)}

def gen():
    cases = []
    for _ in range(N):
        a = random.randint(*RANGES["a"])
        b = random.randint(*RANGES["b"])
        c = random.randint(*RANGES["c"])
        cases.append((f"{a}\n{b}\n{c}\n", str(pow(a, b, c))))
    return cases

def main():
    if len(sys.argv) != 2:
        print(f"usage: {sys.argv[0]} <file.bf>")
        sys.exit(1)

    path = sys.argv[1]
    cases = gen()
    passed = 0

    for i, (inp, expected) in enumerate(cases):
        try:
            proc = subprocess.run(
                ["bfci", "-x", path],
                input=inp, capture_output=True, text=True, timeout=TIMEOUT,
            )
        except subprocess.TimeoutExpired:
            print(f"FAIL #{i+1}: timed out ({TIMEOUT}s)")
            continue

        actual = proc.stdout.strip()

        if proc.returncode != 0:
            print(f"FAIL #{i+1}: exit code {proc.returncode}")
            print(f"  stderr: {proc.stderr.strip()}")
            continue

        if actual == expected:
            passed += 1
        else:
            a, b, c = inp.strip().split("\n")
            print(f"FAIL #{i+1}: pow({a}, {b}, {c})")
            print(f"  expected: {expected}")
            print(f"  got:      {actual!r}")

    print(f"\n{passed}/{N} passed")
    sys.exit(0 if passed == N else 1)

if __name__ == "__main__":
    main()
