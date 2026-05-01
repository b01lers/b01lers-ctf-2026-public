#!/usr/bin/env python3
from __future__ import annotations

import math
import os
import re
import select
import socket
import sys
import time
from dataclasses import dataclass
from typing import Dict, Optional, Tuple

HolElem = Tuple[int, int]


def _env_float(name: str, default: float) -> float:
    raw = os.getenv(name)
    if raw is None:
        return default
    try:
        v = float(raw)
    except ValueError:
        return default
    return v if v > 0 else default


READ_POLL_TIMEOUT = _env_float("SOLVER_READ_POLL_TIMEOUT", 5.0)
QUERY_INT_TIMEOUT = _env_float("SOLVER_QUERY_INT_TIMEOUT", 30.0)
ROUND_HANDLES_TIMEOUT = _env_float("SOLVER_ROUND_HANDLES_TIMEOUT", 60.0)
ROUND_STATUS_TIMEOUT = _env_float("SOLVER_ROUND_STATUS_TIMEOUT", 30.0)


# =========================
# SOCKET-BASED IO
# =========================
class ChallengeIO:
    def __init__(self, host: str, port: int):
        self.sock = socket.create_connection((host, port))
        self.sock.setblocking(False)

        self._buf = ""
        self._pos = 0

    def _read_some(self, timeout: float = 5.0) -> str:
        ready, _, _ = select.select([self.sock], [], [], timeout)
        if not ready:
            raise TimeoutError("timeout waiting for challenge output")

        chunk = self.sock.recv(4096)
        if not chunk:
            raise EOFError("connection closed")

        text = chunk.decode(errors="replace")
        self._buf += text
        return text

    def _compact(self):
        if self._pos > 65536:
            self._buf = self._buf[self._pos :]
            self._pos = 0

    def read_until_any(self, tokens, timeout):
        deadline = time.time() + timeout

        while True:
            segment = self._buf[self._pos :]
            for tok in tokens:
                idx = segment.find(tok)
                if idx >= 0:
                    end = self._pos + idx + len(tok)
                    out = self._buf[self._pos:end]
                    self._pos = end
                    self._compact()
                    return out

            if time.time() > deadline:
                raise TimeoutError("timeout waiting for token")

            self._read_some(timeout=READ_POLL_TIMEOUT)

    def read_next_int_line(self, timeout=QUERY_INT_TIMEOUT):
        deadline = time.time() + timeout
        pat = re.compile(r"(?:^|\n|bb>\s*)(-?\d+)\s*\n")

        while True:
            segment = self._buf[self._pos :]
            m = pat.search(segment)
            if m:
                self._pos += m.end()
                self._compact()
                return int(m.group(1))

            if time.time() > deadline:
                raise TimeoutError("timeout waiting for int")

            self._read_some()

    def read_next_handles(self, timeout=ROUND_HANDLES_TIMEOUT):
        deadline = time.time() + timeout

        handles_re = re.compile(r"one=(\d+)\s+g=(\d+)\s+c=(\d+)\s+h=(\d+)")
        bound_re = re.compile(r"\[\s*0\s*,\s*(\d+)\s*\]")

        while True:
            segment = self._buf[self._pos :]
            hm = handles_re.search(segment)

            if hm:
                prefix = segment[: hm.start()]
                bm = bound_re.search(prefix)
                bound = int(bm.group(1)) if bm else 200

                self._pos += hm.end()
                self._compact()

                return RoundInfo(*map(int, hm.groups()), bound)

            if time.time() > deadline:
                raise TimeoutError("timeout waiting for handles")

            self._read_some()

    def sendline(self, line: str):
        self.sock.sendall((line + "\n").encode())

    def close(self):
        try:
            self.sock.close()
        except Exception:
            pass


# =========================
# BLACK BOX
# =========================
class BlackBoxClient:
    def __init__(self, io: ChallengeIO):
        self.io = io
        self.mul_cache = {}
        self.inv_cache = {}
        self.eq_cache = {}
        self.query_total = 0
        self.query_by_op = {"mul": 0, "inv": 0, "eq": 0}

    def _query_int(self, cmd):
        op = cmd.split()[0]
        if op in self.query_by_op:
            self.query_by_op[op] += 1
        self.query_total += 1

        self.io.sendline(cmd)
        return self.io.read_next_int_line()

    def mul(self, a, b):
        if (a, b) not in self.mul_cache:
            self.mul_cache[(a, b)] = self._query_int(f"mul {a} {b}")
        return self.mul_cache[(a, b)]

    def inv(self, a):
        if a not in self.inv_cache:
            self.inv_cache[a] = self._query_int(f"inv {a}")
        return self.inv_cache[a]

    def eq(self, a, b):
        key = tuple(sorted((a, b)))
        if key not in self.eq_cache:
            self.eq_cache[key] = self._query_int(f"eq {a} {b}") == 1
        return self.eq_cache[key]


# =========================
# MATH
# =========================
@dataclass
class RoundInfo:
    one: int
    g: int
    c: int
    h: int
    bound: int


def hol_mul(bb, x, y):
    a, c = x
    b, d = y
    c_inv = bb.inv(c)
    return (
        bb.mul(a, bb.mul(bb.mul(c, b), c_inv)),
        bb.mul(c, d),
    )


def hol_inv(bb, x):
    a, c = x
    c_inv = bb.inv(c)
    return (
        bb.mul(c_inv, bb.mul(bb.inv(a), c)),
        c_inv,
    )


def hol_pow(bb, base, e, one):
    res = (one, one)
    cur = base
    while e:
        if e & 1:
            res = hol_mul(bb, res, cur)
        cur = hol_mul(bb, cur, cur)
        e >>= 1
    return res


def s_eval(bb, g, c, x, one):
    return hol_pow(bb, (g, c), x, one)[0]


# =========================
# BSGS
# =========================
def recover(bb, g, c, h, one, bound):
    # compute order of c
    cur = one
    ord_c = 1
    while True:
        cur = bb.mul(cur, c)
        if bb.eq(cur, one):
            break
        ord_c += 1
        if ord_c > 2000:
            raise ValueError("order too large")

    base = (g, c)

    # BSGS precomputation
    n = int(math.sqrt(bound)) + 1

    table = {}
    cur = (one, one)
    for j in range(n):
        table[cur] = j
        cur = hol_mul(bb, cur, base)

    factor = hol_inv(bb, hol_pow(bb, base, n, one))

    # iterate over possible second components
    c_pow = one
    for _ in range(ord_c):
        cur = (h, c_pow)

        for i in range(n):
            if cur in table:
                x = i * n + table[cur]
                if 0 <= x <= bound:
                    if bb.eq(s_eval(bb, g, c, x, one), h):
                        return x
            cur = hol_mul(bb, cur, factor)

        c_pow = bb.mul(c_pow, c)

    raise ValueError("no solution")

# =========================
# MAIN
# =========================
def main():
    if len(sys.argv) >= 3:
        host = sys.argv[1]
        port = int(sys.argv[2])
    else:
        host = os.getenv("CHALLENGE_HOST")
        port = int(os.getenv("CHALLENGE_PORT"))

    io = ChallengeIO(host, port)

    try:
        for r in range(1, 6):
            info = io.read_next_handles()
            bb = BlackBoxClient(io)

            x = recover(bb, info.g, info.c, info.h, info.one, info.bound)

            print(f"[+] round {r}: x={x}")
            io.sendline(f"submit {x}")

            status = io.read_until_any(
                ("correct", "wrong", "failed"),
                timeout=ROUND_STATUS_TIMEOUT,
            )

            if "correct" not in status:
                print(status)
                return

        print(io.read_until_any(("}", "flag"), 10))

    finally:
        io.close()


if __name__ == "__main__":
    main()