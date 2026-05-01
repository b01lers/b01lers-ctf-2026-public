#include <immintrin.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/syscall.h>
#include <unistd.h>

// TODO: remove, shouldn't do anything for actual build
#ifndef SYS_arch_prctl
#define SYS_arch_prctl 0x1
#endif
#define ARCH_GET_XCOMP_PERM 0x1022
#define ARCH_REQ_XCOMP_PERM 0x1023
#define XFEATURE_XTILECFG 17
#define XFEATURE_XTILEDATA 18

#include "gen.h"

typedef uint16_t u16;
typedef int8_t i8;
typedef int32_t i32;
typedef struct {
    int height;
    int width;
} shape;

// pad to 16 rows
i8 state[48][TILES] = {0};
i8 next_state[48 * TILES] = {0};

/* Set_tiledata_use() - Invoke syscall to set ARCH_SET_STATE_USE */
__attribute__((always_inline)) bool set_tiledata_use() {
    if (syscall(SYS_arch_prctl, ARCH_REQ_XCOMP_PERM, XFEATURE_XTILEDATA) ||
        syscall(SYS_arch_prctl, ARCH_REQ_XCOMP_PERM, XFEATURE_XTILECFG)) {
        return false;
    }
    return true;
}

typedef struct {
    i8 palette;
    i8 start_row;
    i8 reserved0[14];
    u16 bytes_per_row[16];
    i8 rows[16];
} tilecfg;

// VNNI layout for RHS of multiplication:
// row 0 col 0, row 1 col 0, .. row 3 col 0, ..., row 0 col n, row 1 col n, .. row 3 col n,
// row 4 col 0, row 5 col 0, .. row 6 col 0, ...
// ...
__attribute__((always_inline)) void vnni(shape s, const i8* src, i8* dst) {
    for (int row = 0; row < s.height; row++) {
        for (int col = 0; col < s.width; col++) {
            int wide_row = row / 4;
            int offset = row % 4;
            dst[wide_row * (s.width * 4) + col * 4 + offset] = src[s.width * row + col];
        }
    }
}

__attribute__((always_inline)) void unvnni(shape s, const i8* src, i8* dst) {
    for (int row = 0; row < s.height; row++) {
        for (int col = 0; col < s.width; col++) {
            int wide_row = row / 4;
            int offset = row % 4;
            dst[s.width * row + col] = src[wide_row * (s.width * 4) + col * 4 + offset];
        }
    }
}

__attribute__((always_inline)) void save_state(int chunk, i8* dst) {
    i32 chunk_i32[16][16] = {0};
    _tile_stored(7, chunk_i32, 64);
#pragma unroll
    for (int r = 0; r < 16; r++) {
#pragma unroll
        for (int c = 0; c < 16; c++) {
            dst[(chunk * 16 + r) * 16 + c] = (i8)chunk_i32[r][c];
        }
    }
}

__attribute__((always_inline)) int fromhex(char c) {
    if ('0' <= c && c <= '9') {
        return c - '0';
    }
    if ('a' <= c && c <= 'f') {
        return c - 'a' + 10;
    }
    if ('A' <= c && c <= 'F') {
        return c - 'A' + 10;
    }
    return -1;
}

__attribute__((always_inline)) bool shift(int tile, int dir) {
    const i8* shifter = shifts[tile / 8][dir];
    const i8* mask = masks[tile];
    const i8* inv_mask = inv_masks[tile];

    // operation: shift[chunk*16:(chunk+1)*16, :] @ state @ mask + state[chunk*16:(chunk+1)*16, :] @ inv_mask
    for (int chunk_count = 0; chunk_count < 3; chunk_count++) {
        _tile_zero(7);

        _tile_loadd(3, mask, 64);
        for (int state_section = 0; state_section < 3; state_section++) {
            _tile_zero(6);
            _tile_loadd(0, (i8*)state + state_section * 256, 16);
            _tile_dpbssd(6, 0, 3);  // state[section] @ mask

            i32 chunk[16 * 16];
            _tile_stored(6, chunk, 64);
            i8 small[16 * 16];
            for (int i = 0; i < 256; i++) {
                small[i] = (i8)chunk[i];
            }

            i8 small_vnni[256];
            vnni((shape){16, 16}, small, small_vnni);
            _tile_loadd(4, small_vnni, 64);

            _tile_loadd(1, shifter + (chunk_count * 3 + state_section) * 256, 16);
            _tile_dpbssd(7, 1, 4);  // shifter[chunk, section] @ (state[section] @ mask)

            _tile_stored(7, chunk, 16 * 4);
            for (int i = 0; i < 256; i++) {
                small[i] = (i8)chunk[i];
            }
        }

        _tile_loadd(2, (i8*)state + chunk_count * 256, 16);
        _tile_loadd(4, inv_mask, 64);
        _tile_dpbssd(7, 2, 4);  // state[chunk] @ inv_mask

        save_state(chunk_count, next_state);
    }

    memcpy(state, next_state, sizeof(next_state));

    for (int r = 0; r < 36; r++) {
        int sum = 0;
        for (int c = 0; c < 16; c++) {
            if (state[r][c] < 0) {
                return false;
            }
            sum += state[r][c];
        }
        if (sum > 1) {
            return false;
        }
    }
    return true;
}

// TODO: switch to never_inline and misalign so instructions don't show up as readily
__attribute__((always_inline)) bool chall(int num) {
    printf("%d> ", num);
    fflush(stdout);

    char* line = NULL;
    size_t size = 0;
    if (getline(&line, &size, stdin) == -1) {
        return false;
    }
    line[strcspn(line, "\n")] = 0;
    size = strlen(line);

    if ((size & 1) != 0) {
        return false;
    }

    for (int i = 0; i < size; i += 2) {
        int tile = fromhex(line[i]);
        int dir = fromhex(line[i + 1]);
        if (tile < 0 || tile >= TILES || dir < 0 || dir > 3) {
            free(line);
            return false;
        }
        if (!shift(tile, dir)) {
            free(line);
            return false;
        }
    }

    free(line);
    // is the car at row 2, col 5?
    return state[17][0] == 1;
}

__attribute__((always_inline)) void cleanup() { _tile_release(); }

int main() {
    if (!set_tiledata_use()) {
        printf("Cannot run on this CPU\n");
        return 1;
    }

    // tmm0-2: 16x16 lhs
    // tmm3-5: 16x16 rhs
    // tmm6-7: 16x16 result
    const tilecfg config = {.palette = 1,
                            .start_row = 0,
                            .reserved0 = {0},
                            .bytes_per_row = {16, 16, 16, 64, 64, 64, 16 * 4, 16 * 4},
                            // vnni matrices have rows / 4 (?)
                            .rows = {16, 16, 16, 4, 4, 4, 16, 16}};

    _tile_loadconfig(&config);
    for (int i = 0; i < NUM_PUZZLES; i++) {
        memcpy(state, puzzles[i], sizeof(state));
        if (!chall(i)) {
            puts("incorrect");
            cleanup();
            exit(1);
        }
    }

    FILE* f = fopen("flag.txt", "r");
    if (f != NULL) {
        char* line = NULL;
        size_t size = 0;
        if (getline(&line, &size, f) != -1) {
            line[strcspn(line, "\n")] = 0;
            puts(line);
            free(line);
            cleanup();
            exit(0);
        }
    }
    puts(fake_flag);
    cleanup();
    exit(0);
    return 0;
}
