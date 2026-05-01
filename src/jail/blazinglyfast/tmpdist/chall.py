#!/usr/bin/env python3
import ctypes
import os
import re
import subprocess
import sys
import tempfile
import textwrap
import secrets

FLAG_PATH = os.environ.get("FLAG_PATH", "/tmp/.flag")
with open(FLAG_PATH, "r", encoding="utf-8") as _f:
    FLAG = _f.read().strip()
os.unlink(FLAG_PATH)

_PR_SET_DUMPABLE = 4
ctypes.CDLL("libc.so.6", use_errno=True).prctl(_PR_SET_DUMPABLE, 0, 0, 0, 0)
TARGET = os.environ.get("RUST_TARGET", "i686-unknown-linux-gnu")
MAX_INPUT_LEN = 4000
COMPILE_TIMEOUT = 20
RUN_TIMEOUT = 5

_TOOLCHAIN = "/usr/local/rustup/toolchains/stable-x86_64-unknown-linux-gnu"
RUSTC = _TOOLCHAIN + "/bin/rustc"
RUSTC_LIB = _TOOLCHAIN + "/lib"
SYSROOT = _TOOLCHAIN

if not os.path.exists(RUSTC):
    raise RuntimeError(f"rustc not found at {RUSTC}")


RUST_TEMPLATE = r"""
#![allow(dead_code)]
#![allow(unused_variables)]
#![allow(unused_imports)]

mod host {
    #[repr(transparent)]
    #[derive(Clone, Copy)]
    struct GateIn(fn() -> &'static str);

    #[repr(transparent)]
    #[derive(Clone, Copy)]
    struct GateOut(fn() -> &'static str);

    #[repr(transparent)]
    #[derive(Clone, Copy)]
    struct CookieIn(u64);

    #[repr(transparent)]
    #[derive(Clone, Copy)]
    struct CookieOut(u64);

    #[repr(C)]
    #[derive(Clone, Copy)]
    struct SecretIn {
        gate: GateIn,
        cookie: CookieIn,
        folded: u64,
        pair: [u64; 2],
    }

    #[repr(C)]
    #[derive(Clone, Copy)]
    struct SecretOut {
        gate: GateOut,
        cookie: CookieOut,
        folded: u64,
        pair: [u64; 2],
    }

    #[repr(C)]
    pub struct In(SecretIn);

    #[repr(C)]
    pub struct Out(SecretOut);

    fn reveal_token() -> &'static str {
        "__EXPECTED_TOKEN__"
    }

    fn decoy() -> &'static str {
        "nope"
    }

    const COOKIE: u64 = 0x1337_1337_cafe_babe;

    fn mix_a(x: u64) -> u64 {
        x.rotate_left(13) ^ 0x9e37_79b9_7f4a_7c15
    }

    fn mix_b(x: u64) -> u64 {
        x.rotate_right(7) ^ 0x243f_6a88_85a3_08d3
    }

    fn mix_c(x: u64) -> u64 {
        x.wrapping_mul(0xbf58_476d_1ce4_e5b9).rotate_left(17)
    }

    pub fn seed() -> In {
        let cookie = COOKIE;
        let a = mix_a(cookie);
        let b = mix_b(cookie);
        let c = mix_c(cookie ^ a ^ b);

        In(SecretIn {
            gate: GateIn(reveal_token),
            cookie: CookieIn(cookie),
            folded: a ^ b ^ c ^ 0xd1b5_4a32_d192_ed03,
            pair: [a ^ 0xa5a5_a5a5_a5a5_a5a5, b ^ c],
        })
    }

    pub fn check(out: Out) {
        let s: SecretOut = unsafe { std::mem::transmute(out) };

        let cookie = s.cookie.0;
        let a = mix_a(cookie);
        let b = mix_b(cookie);
        let c = mix_c(cookie ^ a ^ b);

        let ok_gate = std::ptr::fn_addr_eq(s.gate.0, reveal_token as fn() -> &'static str);
        let ok_cookie = cookie == COOKIE;
        let ok_folded = s.folded == (a ^ b ^ c ^ 0xd1b5_4a32_d192_ed03);
        let ok_pair0 = s.pair[0] == (a ^ 0xa5a5_a5a5_a5a5_a5a5);
        let ok_pair1 = s.pair[1] == (b ^ c);

        if ok_gate && ok_cookie && ok_folded && ok_pair0 && ok_pair1 {
            println!("{}", (s.gate.0)());
        } else if std::ptr::fn_addr_eq(s.gate.0, decoy as fn() -> &'static str) {
            println!(
                "decoy cookie={:#x} folded={:#x} pair={:#x}:{:#x}",
                cookie,
                s.folded,
                s.pair[0],
                s.pair[1]
            );
        } else {
            println!(
                "denied cookie={:#x} folded={:#x} pair={:#x}:{:#x}",
                cookie,
                s.folded,
                s.pair[0],
                s.pair[1]
            );
        }
    }
}

#[forbid(unsafe_code)]
mod usercode {
    use super::host::{In, Out};

    pub fn jail(input: In) -> Out {
__USER_CODE__
    }
}

fn main() {
    let input = host::seed();
    let out = usercode::jail(input);
    host::check(out);
}
""".lstrip()

FORBIDDEN = [
    r"\bunsafe\b",
    r"\bextern\b",
    r"\btrait\b",
    r"\bimpl\b",
    r"\bstd\b",
    r"\#",
    r"\!",
]


def read_user_code() -> str:
    print(f"Target: {TARGET}")
    print("Enter the BODY of `pub fn jail(input: In) -> Out { ... }`")
    print("Finish with EOF (Ctrl+D) or a line containing only 'EOF'.")
    print()
    sys.stdout.flush()

    lines = []
    try:
        for line in sys.stdin:
            if line.rstrip("\r\n") == "EOF":
                break
            lines.append(line)
    except Exception:
        pass
    return "".join(lines)


def read_flag() -> str:
    return FLAG


def validate_user_code(code: str) -> tuple[bool, str]:
    if len(code) > MAX_INPUT_LEN:
        return False, "submission rejected: too long"

    for pat in FORBIDDEN:
        if re.search(pat, code):
            return False, f"submission rejected: forbidden token matched `{pat}`"

    return True, "ok"


def indent_block(code: str, spaces: int = 8) -> str:
    code = code.rstrip()
    if not code:
        code = "loop {}"
    return textwrap.indent(code, " " * spaces)


def build_source(user_code: str, expected_token: str) -> str:
    src = RUST_TEMPLATE.replace("__EXPECTED_TOKEN__", expected_token)
    return src.replace("__USER_CODE__", indent_block(user_code))


def compile_rust(workdir: str) -> tuple[bool, str]:
    main_rs = os.path.join(workdir, "main.rs")
    binary = os.path.join(workdir, "chall32")

    proc = subprocess.run(
        [
            RUSTC,
            "--sysroot",
            SYSROOT,
            "--edition=2021",
            "-O",
            "-C",
            "linker=gcc",
            "--target",
            TARGET,
            main_rs,
            "-o",
            binary,
        ],
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True,
        timeout=COMPILE_TIMEOUT,
        env={**os.environ, "LD_LIBRARY_PATH": RUSTC_LIB},
    )
    output = proc.stderr
    if proc.stdout:
        output = output + ("\n" if output else "") + proc.stdout
    return proc.returncode == 0, output


def run_binary(binary_path: str) -> tuple[bool, str]:
    os.chmod(binary_path, 0o111)
    proc = subprocess.run(
        [binary_path],
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True,
        timeout=RUN_TIMEOUT,
    )
    out = proc.stdout
    if proc.stderr:
        out += "\n[stderr]\n" + proc.stderr
    return proc.returncode == 0, out


def main():
    user_code = read_user_code()

    ok, msg = validate_user_code(user_code)
    if not ok:
        print(msg)
        return

    expected_token = secrets.token_urlsafe(24)
    source = build_source(user_code, expected_token)

    with tempfile.TemporaryDirectory(prefix="rust_jail_") as tmpdir:
        os.chmod(tmpdir, 0o700)
        main_rs = os.path.join(tmpdir, "main.rs")
        binary = os.path.join(tmpdir, "chall32")

        with open(main_rs, "w", encoding="utf-8") as f:
            f.write(source)

        try:
            ok, compile_output = compile_rust(tmpdir)
        except subprocess.TimeoutExpired:
            print("compilation timed out")
            return

        if not ok:
            print("compilation failed:")
            print(compile_output)
            return

        if not os.path.isfile(binary):
            print("compilation failed: no binary produced")
            print(compile_output)
            return

        # Remove source and any compiler artifacts so the running binary
        # cannot read the embedded token from them.
        for name in os.listdir(tmpdir):
            path = os.path.join(tmpdir, name)
            if path != binary:
                os.unlink(path)

        try:
            ok, run_output = run_binary(binary)
        except subprocess.TimeoutExpired:
            print("execution timed out")
            return

        normalized = run_output.strip()
        if ok and normalized == expected_token:
            print(read_flag())
        else:
            print(run_output)


if __name__ == "__main__":
    main()
