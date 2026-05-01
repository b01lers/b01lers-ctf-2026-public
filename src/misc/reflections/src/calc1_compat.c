#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static uint32_t stack[1024];
static size_t sp;

static int read_byte(void) {
    int c = getchar();
    if (c == EOF) {
        exit(0);
    }
    return c & 0xff;
}

static int gethex(void);

static int convhex(int c) {
    int v = c - 0x30;
    if (v < 0) {
        return gethex();
    }
    if (v >= 0x30) {
        v -= 0x27;
    }
    return v;
}

static int gethex(void) {
    for (;;) {
        int c = read_byte();
        if (c <= 0x20) {
            continue;
        }
        if (c == '#') {
            do {
                c = read_byte();
            } while (c != '\n');
            continue;
        }
        return convhex(c);
    }
}

static void output_byte(uint32_t value) {
    if (putchar((int)(value & 0xff)) == EOF) {
        exit(1);
    }
}

int main(void) {
    for (;;) {
        int c = read_byte();
        uint32_t a;
        uint32_t b;

        if (c <= 0x20) {
            continue;
        }
        if (c == '#') {
            do {
                c = read_byte();
            } while (c != '\n');
            continue;
        }
        if (c == '!') {
            stack[sp++] = (uint32_t)read_byte();
            continue;
        }
        if (c == '$') {
            stack[sp++] = (uint32_t)((gethex() << 4) + gethex());
            continue;
        }
        if (c == '*') {
            b = stack[--sp];
            a = stack[--sp];
            stack[sp++] = a * b;
            continue;
        }
        if (c == '+') {
            b = stack[--sp];
            a = stack[--sp];
            stack[sp++] = a + b;
            continue;
        }
        if (c == '-') {
            b = stack[--sp];
            a = stack[--sp];
            stack[sp++] = a - b;
            continue;
        }
        if (c == '/') {
            int32_t divisor;
            int32_t dividend;
            b = stack[--sp];
            a = stack[--sp];
            divisor = (int32_t)b;
            dividend = (int32_t)a;
            stack[sp++] = (uint32_t)(dividend / divisor);
            continue;
        }
        if (c == '@') {
            output_byte(stack[--sp]);
            continue;
        }

        output_byte((uint32_t)((convhex(c) << 4) + gethex()));
    }
}
