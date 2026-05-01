import numpy as np
from pathlib import Path

board_size = 6
padded = 48
num_pieces = 16
fake_flag = "flag{fake_flag}"


def chunked_16x16(arr: np.matrix) -> bytes:
    assert arr.dtype == np.int8
    assert all(s % 16 == 0 for s in arr.shape)
    return b"".join(arr[r * 16 : (r + 1) * 16, c * 16 : (c + 1) * 16].tobytes("C") for r in range(3) for c in range(3))


def standard(arr: np.matrix) -> bytes:
    assert arr.dtype == np.int8
    return arr.tobytes("C")


def vnni(arr: np.matrix) -> bytes:
    assert arr.dtype == np.int8
    res = []
    assert arr.shape[0] % 4 == 0
    for row in range(0, arr.shape[0], 4):
        chunk = arr[row : row + 4, :].T.flatten()
        res.append(chunk.tobytes("C"))
    return b"".join(res)


def filled(shape: tuple[int, ...], val) -> np.matrix:
    a = np.empty(shape, dtype=np.int8)
    a.fill(val)
    return np.asmatrix(a)


def clamp(i):
    return min(max(0, i), board_size)


def lr(dir: int):
    chunk = np.zeros((board_size, board_size), dtype=np.int8)
    chunk[1:, :-1] = np.eye(board_size - 1)
    chunk[board_size - 1, board_size - 1] = 1
    if dir == -1:
        chunk = np.flip(np.flip(chunk, axis=1), axis=0)

    blocks = np.zeros((padded, padded), dtype=np.int8)
    for i in range(board_size):
        blocks[board_size * i : board_size * (i + 1), board_size * i : board_size * (i + 1)] = chunk
    return np.asmatrix(blocks)


def ud(dir: int):
    ident = np.eye(board_size, dtype=np.int8)
    blocks = np.zeros((padded, padded), dtype=np.int8)
    if dir == 1:
        blocks[board_size * (board_size - 1) : board_size**2, board_size * (board_size - 1) : board_size**2] = ident
        for i in range(0, board_size - 1):
            blocks[board_size * (i + 1) : board_size * (i + 2), board_size * i : board_size * (i + 1)] = ident
    else:
        blocks[0:board_size, 0:board_size] = ident
        for i in range(0, board_size - 1):
            blocks[board_size * i : board_size * (i + 1), board_size * (i + 1) : board_size * (i + 2)] = ident
    return np.asmatrix(blocks)


# np.set_printoptions(linewidth=120, threshold=36 * 36)
# print(lr(1)[:36, :36])
# print(lr(1)[: board_size**2, : board_size**2])
# print(lr(-1)[: board_size**2, : board_size**2])
# print(ud(1)[: board_size**2, : board_size**2])
# print(ud(-1)[: board_size**2, : board_size**2])

perm_matrices = [lr(-1), lr(1), ud(-1), ud(1)]

# test = np.matrix(np.zeros((1, 48)).T)
# test[25:28, :] = 1
# print(test)
# print(lr(-1)[:36, :36])
# print(np.nonzero(test))
# res = lr(-1) @ test
# print(res)
# print(np.nonzero(res))


def bytes_to_c(b: bytes):
    return str(list(b)).replace("[", "{").replace("]", "}")


def c_list(arr: list[str]):
    return "{" + ", ".join(arr) + "}"


def mask(pos: int) -> np.matrix:
    arr = np.zeros((num_pieces, num_pieces))
    arr[pos][pos] = 1
    return np.asmatrix(arr.astype(np.int8))


def inv_mask(pos: int) -> np.matrix:
    arr = np.eye(num_pieces)
    arr[pos][pos] = 0
    return np.asmatrix(arr.astype(np.int8))

def make_puzzle(pzl: str) -> np.matrix:
    res = np.asmatrix(np.zeros((padded, num_pieces), dtype=np.int8))
    for i, c in enumerate(pzl.replace("\n", "")):
        if c != ".":
            res[i, int(c, 16)] = 1
    return res


puzzles = [make_puzzle(p) for p in Path("puzzles.txt").read_text().strip().split("\n\n")]

with Path("gen.h").open("w") as f:
    f.write("#ifndef GEN_H\n#define GEN_H\n")
    f.write(f"""#include <stdint.h>
typedef uint16_t u16;
typedef int8_t i8;
typedef int32_t i32;
#define TILES ({num_pieces})\n""")

    f.write(
        f"const i8 masks[{num_pieces}][{num_pieces**2}] = {c_list([bytes_to_c(vnni(mask(i))) for i in range(num_pieces)])};\n"
    )
    f.write(
        f"const i8 inv_masks[{num_pieces}][{num_pieces**2}] = {c_list([bytes_to_c(vnni(inv_mask(i))) for i in range(num_pieces)])};\n"
    )

    f.write("// shifts[0]: l/r, shifts[1]: u/d\n")

    a = [  # lr
        bytes_to_c(chunked_16x16(perm_matrices[0])),
        bytes_to_c(chunked_16x16(perm_matrices[1])),
        bytes_to_c(chunked_16x16(filled((padded, padded), 7))),
        bytes_to_c(chunked_16x16(filled((padded, padded), 7))),
    ]
    b = [  # ud
        bytes_to_c(chunked_16x16(filled((padded, padded), 7))),
        bytes_to_c(chunked_16x16(filled((padded, padded), 7))),
        bytes_to_c(chunked_16x16(perm_matrices[2])),
        bytes_to_c(chunked_16x16(perm_matrices[3])),
    ]

    f.write(f"const i8 shifts[2][4][{padded}*{padded}] = {c_list([c_list(a), c_list(b)])};\n")
    f.write(f'const char* const fake_flag = "{fake_flag}";\n')
    f.write(f"const int counts[{len(puzzles)}] = {c_list([str(np.count_nonzero(p)) for p in puzzles])};\n")

    #
    # define NUM_PUZZLES 1
    # i8 puzzles[NUM_PUZZLES][48][16] = {0};
    f.write(f"#define NUM_PUZZLES {len(puzzles)}\n")
    f.write(f"const i8 puzzles[NUM_PUZZLES][48*16] = {c_list([bytes_to_c(standard(p)) for p in puzzles])};\n")

    f.write("#endif\n")
