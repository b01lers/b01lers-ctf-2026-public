#!/usr/bin/env python3
import argparse
import math
import re
import socket

from cryptography.hazmat.primitives.ciphers.aead import AESGCM

BLOCK_SIZE = 16
WORD_MASK = 0xFFFFFFFF
STATE_SIZE = 624
GCM_REDUCTION = 0xE1000000000000000000000000000000
GF128_MASK = (1 << 128) - 1
LOW64_MASK = (1 << 64) - 1
RECOVERY_START = 620
PREDICTION_WORDS = 36


def recv_until(sock, marker=b"> "):
    data = b""
    while marker not in data:
        chunk = sock.recv(4096)
        if not chunk:
            break
        data += chunk
    return data


def parse_query_block(text):
    nonce = bytes.fromhex(re.search(r"nonce = ([0-9a-f]+)", text).group(1))
    ciphertext = bytes.fromhex(re.search(r"ciphertext = ([0-9a-f]+)", text).group(1))
    tag = int(re.search(r"tag = ([0-9a-f]+)", text).group(1), 16)
    return nonce, ciphertext, tag


def collect_session(host, port):
    sock = socket.create_connection((host, port))
    banner = recv_until(sock).decode()

    flag_nonce = bytes.fromhex(re.search(r"flag_nonce = ([0-9a-f]+)", banner).group(1))
    flag_ciphertext = bytes.fromhex(re.search(r"flag_ciphertext = ([0-9a-f]+)", banner).group(1))
    flag_tag = bytes.fromhex(re.search(r"flag_tag = ([0-9a-f]+)", banner).group(1))
    query_budget = int(re.search(r"query budget = (\d+)", banner).group(1))

    queries = []
    for _ in range(query_budget):
        sock.sendall(b"1\n")
        queries.append(parse_query_block(recv_until(sock).decode()))

    sock.sendall(b"2\n")
    sock.close()
    return flag_nonce, flag_ciphertext, flag_tag, queries


def gf_mul(x, y):
    z = 0
    v = x
    for i in range(128):
        if (y >> (127 - i)) & 1:
            z ^= v
        if v & 1:
            v = (v >> 1) ^ GCM_REDUCTION
        else:
            v >>= 1
    return z & GF128_MASK


def one_block_ghash(h, ciphertext_block):
    return gf_mul(gf_mul(ciphertext_block, h) ^ 128, h)


_linear_cache = {}


def linear_rows_for_ciphertext(ciphertext_block):
    rows = _linear_cache.get(ciphertext_block)
    if rows is not None:
        return rows

    rows = [0] * 128
    for bit_index in range(128):
        out = one_block_ghash(1 << bit_index, ciphertext_block)
        for out_bit in range(128):
            if (out >> out_bit) & 1:
                rows[out_bit] |= 1 << bit_index

    _linear_cache[ciphertext_block] = rows
    return rows


def lower_tag_word_expressions(tag, ciphertext):
    rows = linear_rows_for_ciphertext(int.from_bytes(ciphertext, "big"))
    words = []
    for word_index in (2, 3):
        observed = (tag >> (96 - 32 * word_index)) & WORD_MASK
        bits = []
        for bit in range(32):
            expr = rows[96 - 32 * word_index + bit]
            if (observed >> bit) & 1:
                expr |= 1 << 128
            bits.append(expr)
        words.append(bits)
    return words


def xor_word(left, right):
    return [a ^ b for a, b in zip(left, right)]


def right_shift(word, count):
    return [word[i + count] if i + count < 32 else 0 for i in range(32)]


def left_shift_and(word, count, mask):
    return [word[i - count] if i - count >= 0 and ((mask >> i) & 1) else 0 for i in range(32)]


def temper_symbolic(word):
    y = xor_word(word, right_shift(word, 11))
    y = xor_word(y, left_shift_and(y, 7, 0x9D2C5680))
    y = xor_word(y, left_shift_and(y, 15, 0xEFC60000))
    y = xor_word(y, right_shift(y, 18))
    return y


def undo_right_xor(word, count):
    out = [0] * 32
    for i in range(31, -1, -1):
        out[i] = word[i] ^ (out[i + count] if i + count < 32 else 0)
    return out


def undo_left_xor(word, count, mask):
    out = [0] * 32
    for i in range(32):
        out[i] = word[i] ^ (out[i - count] if i - count >= 0 and ((mask >> i) & 1) else 0)
    return out


def untemper_symbolic(word):
    x = undo_right_xor(word, 18)
    x = undo_left_xor(x, 15, 0xEFC60000)
    x = undo_left_xor(x, 7, 0x9D2C5680)
    x = undo_right_xor(x, 11)
    return x


def twist_symbolic_state(state):
    out = []
    for i in range(STATE_SIZE):
        y = [state[(i + 1) % STATE_SIZE][bit] if bit < 31 else state[i][31] for bit in range(32)]
        y_shifted = [y[bit + 1] if bit + 1 < 32 else 0 for bit in range(32)]
        matrix_term = [y[0] if ((0x9908B0DF >> bit) & 1) else 0 for bit in range(32)]
        out.append(xor_word(xor_word(state[(i + 397) % STATE_SIZE], y_shifted), matrix_term))
    return out


def solve_linear_system(equations):
    basis = [0] * 128
    coeff_mask = (1 << 128) - 1

    for equation in equations:
        coeffs = equation & coeff_mask
        for pivot in range(127, -1, -1):
            if not ((coeffs >> pivot) & 1):
                continue
            if basis[pivot]:
                equation ^= basis[pivot]
                coeffs = equation & coeff_mask
            else:
                basis[pivot] = equation
                break
        else:
            if equation >> 128:
                raise ValueError("inconsistent linear system")

    if sum(1 for row in basis if row) < 128:
        raise ValueError("rank-deficient linear system")

    solution = 0
    for pivot in range(128):
        row = basis[pivot]
        lower_terms = row & ((1 << pivot) - 1)
        bit = ((row >> 128) & 1) ^ ((lower_terms & solution).bit_count() & 1)
        solution |= bit << pivot
    return solution


class ReversibleMT19937:
    def __init__(self):
        self.state = None

    @staticmethod
    def _upper(value):
        return value & 0x80000000

    @staticmethod
    def _lower(value):
        return value & 0x7FFFFFFF

    @staticmethod
    def _times_a(value):
        return ((value >> 1) ^ (0x9908B0DF if value & 1 else 0)) & WORD_MASK

    @staticmethod
    def _invert_times_a(value):
        if value & 0x80000000:
            return (((value ^ 0x9908B0DF) << 1) | 1) & WORD_MASK
        return (value << 1) & WORD_MASK

    @staticmethod
    def _invert_xor_shift_and(number, shift_right, mask):
        rounds = math.ceil(32 / abs(shift_right)) - 1
        intermediate = number
        for _ in range(rounds):
            if shift_right > 0:
                intermediate >>= shift_right
            else:
                intermediate = (intermediate << (-shift_right)) & WORD_MASK
            intermediate = number ^ (intermediate & mask)
        return intermediate & WORD_MASK

    def _untemper(self, value):
        y3 = self._invert_xor_shift_and(value, 18, WORD_MASK)
        y2 = self._invert_xor_shift_and(y3, -15, 0xEFC60000)
        y1 = self._invert_xor_shift_and(y2, -7, 0x9D2C5680)
        return self._invert_xor_shift_and(y1, 11, WORD_MASK)

    def clone_state_from_output(self, outputs):
        if len(outputs) != STATE_SIZE:
            raise ValueError("need exactly 624 raw MT outputs")
        self.state = [self._untemper(value) for value in outputs]

    def _untwist_one(self):
        x_a = self.state[-1] ^ self.state[396]
        x = self._invert_times_a(x_a)
        prev_x_a = self.state[-2] ^ self.state[395]
        prev_x = self._invert_times_a(prev_x_a)
        self.state.insert(0, (self._upper(x) | self._lower(prev_x)) & WORD_MASK)
        self.state.pop()

    def rewind(self, rounds):
        for _ in range(rounds):
            self._untwist_one()


def init_genrand(seed=19650218):
    state = [0] * STATE_SIZE
    state[0] = seed & WORD_MASK
    for i in range(1, STATE_SIZE):
        state[i] = (1812433253 * (state[i - 1] ^ (state[i - 1] >> 30)) + i) & WORD_MASK
    return state


def mix_first_loop(value):
    return ((value ^ (value >> 30)) * 1664525) & WORD_MASK


def mix_second_loop(value):
    return ((value ^ (value >> 30)) * 1566083941) & WORD_MASK


def recover_seed_words(pre_output_state):
    current = pre_output_state[:]
    intermediate = [0] * STATE_SIZE

    intermediate[1] = ((current[1] + 1) & WORD_MASK) ^ mix_second_loop(current[623])
    intermediate[2] = ((current[2] + 2) & WORD_MASK) ^ mix_second_loop(intermediate[1])
    for i in range(3, STATE_SIZE):
        intermediate[i] = ((current[i] + i) & WORD_MASK) ^ mix_second_loop(current[i - 1])
    intermediate[0] = intermediate[623]

    seeded = init_genrand(19650218)
    words = [None] * 8
    for i in range(3, STATE_SIZE):
        word_index = (i - 1) % 8
        candidate = (intermediate[i] - (seeded[i] ^ mix_first_loop(intermediate[i - 1])) - word_index) & WORD_MASK
        if words[word_index] is None:
            words[word_index] = candidate
        elif words[word_index] != candidate:
            raise ValueError("seed recovery inconsistency")

    first_update_word1 = ((intermediate[1] - words[7] - 7) & WORD_MASK) ^ mix_first_loop(intermediate[0])
    words[0] = (first_update_word1 - (seeded[1] ^ mix_first_loop(seeded[0]))) & WORD_MASK
    words[1] = (intermediate[2] - (seeded[2] ^ mix_first_loop(first_update_word1)) - 1) & WORD_MASK
    return words


def extract_fault_words(ghash_value, faulty_tag):
    lower = (ghash_value ^ faulty_tag) & LOW64_MASK
    return [lower >> 32, lower & WORD_MASK]


def recover_h(queries):
    word_expressions = []
    for _, ciphertext, tag in queries:
        word_expressions.extend(lower_tag_word_expressions(tag, ciphertext))

    symbolic_state = [untemper_symbolic(word) for word in word_expressions[RECOVERY_START:RECOVERY_START + STATE_SIZE]]
    symbolic_next = twist_symbolic_state(symbolic_state)
    predicted_words = [temper_symbolic(word) for word in symbolic_next[:PREDICTION_WORDS]]
    observed_words = word_expressions[RECOVERY_START + STATE_SIZE:RECOVERY_START + STATE_SIZE + PREDICTION_WORDS]
    equations = [predicted_words[i][bit] ^ observed_words[i][bit] for i in range(len(observed_words)) for bit in range(32)]
    return solve_linear_system(equations)


def recover_master_key(queries, h):
    raw_words = []
    for _, ciphertext, tag in queries:
        ghash_value = one_block_ghash(h, int.from_bytes(ciphertext, "big"))
        raw_words.extend(extract_fault_words(ghash_value, tag))

    mt = ReversibleMT19937()
    mt.clone_state_from_output(raw_words[RECOVERY_START:RECOVERY_START + STATE_SIZE])
    mt.rewind(RECOVERY_START + STATE_SIZE)

    seed_words = recover_seed_words(mt.state)
    seed = sum(word << (32 * index) for index, word in enumerate(seed_words))
    return seed.to_bytes(32, "big")


def decrypt_flag(master_key, nonce, ciphertext, tag):
    return AESGCM(master_key).decrypt(nonce, ciphertext + tag, None)


def solve(host, port):
    flag_nonce, flag_ciphertext, flag_tag, queries = collect_session(host, port)
    h = recover_h(queries)
    master_key = recover_master_key(queries, h)
    return decrypt_flag(master_key, flag_nonce, flag_ciphertext, flag_tag)


def main():
    parser = argparse.ArgumentParser(description="Solve the manytags challenge")
    parser.add_argument("--host", default="127.0.0.1")
    parser.add_argument("--port", type=int, default=1337)
    args = parser.parse_args()

    flag = solve(args.host, args.port)
    print(flag)
    print(flag.decode())


if __name__ == "__main__":
    main()
