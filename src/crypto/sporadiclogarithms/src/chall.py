#!/usr/bin/env python3

from __future__ import annotations

import os
import random
import sys
from dataclasses import dataclass
from typing import Dict, Tuple

from sage.all import GF, MatrixSpace, divisors, identity_matrix


def eprint(msg: str) -> None:
    print(msg, flush=True)


def mat_key(m) -> Tuple[int, ...]:
    return tuple(int(x) for x in m.list())


@dataclass
class Params:
    p: int = int(os.getenv("SAGE_CHALLENGE_P", "65537"))
    n: int = int(os.getenv("SAGE_CHALLENGE_N", "3"))
    rounds: int = int(os.getenv("SAGE_CHALLENGE_ROUNDS", "5"))
    bound: int = int(os.getenv("SAGE_CHALLENGE_BOUND", "262144"))
    max_queries: int = int(os.getenv("SAGE_CHALLENGE_MAX_QUERIES", "10000"))
    max_c_order: int = int(os.getenv("SAGE_CHALLENGE_MAX_C_ORDER", "8"))


class SageBlackBox:
    def __init__(self, M, c) -> None:
        self.M = M
        self.c = c
        self.c_inv = c.inverse()
        self.table = []
        self.index: Dict[Tuple[int, ...], int] = {}

        self.id_elem = identity_matrix(M.base_ring(), M.nrows())
        self.id = self._add_elem(self.id_elem)

    def _add_elem(self, x) -> int:
        k = mat_key(x)
        if k in self.index:
            return self.index[k]
        self.table.append(x)
        h = len(self.table)
        self.index[k] = h
        return h

    def rand(self) -> int:
        while True:
            x = self.M.random_element()
            if x.det() != 0:
                return self._add_elem(x)

    def mul(self, a: int, b: int) -> int:
        return self._add_elem(self.table[a - 1] * self.table[b - 1])

    def inv(self, a: int) -> int:
        return self._add_elem(self.table[a - 1].inverse())

    def eq(self, a: int, b: int) -> bool:
        return self.table[a - 1] == self.table[b - 1]

    def ord(self, a: int) -> int:
        return int(self.table[a - 1].multiplicative_order())

    def phi(self, a: int) -> int:
        x = self.table[a - 1]
        return self._add_elem(self.c * x * self.c_inv)


def choose_small_order_conjugator(F, n: int, max_order: int):
    divs = [d for d in divisors(F.order() - 1) if 1 < int(d) <= max_order]
    if not divs:
        raise RuntimeError("no suitable small order available; increase field size")

    m = int(random.choice(divs))
    g = F.multiplicative_generator()
    d = g ** ((F.order() - 1) // m)

    c = identity_matrix(F, n)
    c[0, 0] = d
    return c, m


def hol_mul(x, y):
    a, c1 = x
    b, c2 = y
    return a * (c1 * b * c1.inverse()), c1 * c2


def hol_pow(base, e, F, n: int):
    one = identity_matrix(F, n)
    result = (one, one)
    cur = base
    k = e
    while k > 0:
        if k & 1:
            result = hol_mul(result, cur)
        cur = hol_mul(cur, cur)
        k >>= 1
    return result


def print_help() -> None:
    eprint("Commands:")
    eprint("  help")
    eprint("  handles               # print one,g,c,h handles")
    eprint("  mul <a> <b>           # return handle of a*b")
    eprint("  inv <a>               # return handle of a^-1")
    eprint("  phi <a>               # return handle of c*a*c^-1")
    eprint("  eq <a> <b>            # return 1 if equal else 0")
    eprint("  submit <x>            # submit exponent x")
    eprint("  quit")


def parse_nonneg_int(tok: str):
    if not tok.isdigit():
        return None
    return int(tok)


def run_round(params: Params, ridx: int) -> bool:
    F = GF(params.p)
    M = MatrixSpace(F, params.n)

    eprint(f"\n=== Round {ridx}/{params.rounds} setup ===")
    eprint("[setup] choosing conjugator c with small order...")
    c, c_order = choose_small_order_conjugator(F, params.n, params.max_c_order)

    bb = SageBlackBox(M, c)
    eprint(f"[setup] c order={c_order}")

    g = bb.rand()
    g_ord = bb.ord(g)
    eprint(f"[setup] sampled g with ord={g_ord}")

    bound = params.bound
    x_secret = random.randint(0, bound)

    g_elem = bb.table[g - 1]
    h_hol = hol_pow((g_elem, c), x_secret, F, params.n)
    h = bb._add_elem(h_hol[0])
    c_handle = bb._add_elem(c)

    eprint(f"Find any x in [0, {bound}] such that h = s_{{g,phi}}(x),")
    eprint("where phi(a) = c*a*c^-1 and s_{g,phi}(x)")
    eprint(f"group=GL({params.n},{params.p}) one={bb.id} g={g} c={c_handle} h={h}")
    print_help()

    queries = 0
    while True:
        print("bb> ", end="", flush=True)
        line = sys.stdin.readline()
        if line == "":
            eprint("EOF")
            return False
        toks = line.strip().split()
        if not toks:
            continue

        cmd = toks[0].lower()

        if cmd == "help":
            print_help()
            continue

        if cmd == "handles":
            eprint(f"one={bb.id} g={g} c={c_handle} h={h}")
            continue

        if cmd == "submit":
            if len(toks) != 2:
                eprint("usage: submit <x>")
                continue
            x = parse_nonneg_int(toks[1])
            if x is None:
                eprint("error: x must be a non-negative integer")
                continue

            hx = hol_pow((g_elem, c), x, F, params.n)[0]
            if hx == bb.table[h - 1]:
                eprint("correct")
                return True
            eprint("wrong")
            return False

        if cmd == "quit":
            return False

        if cmd in ("mul", "inv", "phi", "eq"):
            if queries >= params.max_queries:
                eprint("query limit exceeded")
                return False
            queries += 1

        if cmd == "mul":
            if len(toks) != 3:
                eprint("usage: mul <a> <b>")
                continue
            a = parse_nonneg_int(toks[1])
            b = parse_nonneg_int(toks[2])
            if a is None or b is None:
                eprint("error: handles must be non-negative integers")
                continue
            if a < 1 or b < 1 or a > len(bb.table) or b > len(bb.table):
                eprint("error: unknown handle")
                continue
            eprint(str(bb.mul(a, b)))
            continue

        if cmd == "inv":
            if len(toks) != 2:
                eprint("usage: inv <a>")
                continue
            a = parse_nonneg_int(toks[1])
            if a is None:
                eprint("error: handle must be a non-negative integer")
                continue
            if a < 1 or a > len(bb.table):
                eprint("error: unknown handle")
                continue
            eprint(str(bb.inv(a)))
            continue

        if cmd == "phi":
            if len(toks) != 2:
                eprint("usage: phi <a>")
                continue
            a = parse_nonneg_int(toks[1])
            if a is None:
                eprint("error: handle must be a non-negative integer")
                continue
            if a < 1 or a > len(bb.table):
                eprint("error: unknown handle")
                continue
            eprint(str(bb.phi(a)))
            continue

        if cmd == "eq":
            if len(toks) != 3:
                eprint("usage: eq <a> <b>")
                continue
            a = parse_nonneg_int(toks[1])
            b = parse_nonneg_int(toks[2])
            if a is None or b is None:
                eprint("error: handles must be non-negative integers")
                continue
            if a < 1 or b < 1 or a > len(bb.table) or b > len(bb.table):
                eprint("error: unknown handle")
                continue
            eprint("1" if bb.eq(a, b) else "0")
            continue

        eprint("unknown command (type help)")


def main() -> int:
    params = Params()
    flag = os.getenv("FLAG", "flag{set_FLAG_env}")

    eprint(f"Pass {params.rounds} rounds to get the flag.")
    for i in range(1, params.rounds + 1):
        if not run_round(params, i):
            eprint("Challenge failed.")
            return 1

    eprint(flag)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
