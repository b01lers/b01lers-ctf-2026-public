#!/usr/bin/env python3
from __future__ import annotations

import argparse
import base64
import os
import subprocess
from pathlib import Path

from pwn import context, remote


PROMPT = b"~ $ "


def solve_pow(io) -> None:
    data = io.recvuntil(b"solution: ")
    text = data.decode(errors="replace")
    lines = text.splitlines()
    cmd = lines[0]

    print(f"solving proof of work with: {cmd}")
    sol = subprocess.check_output(cmd, shell=True, text=True).strip()
    io.sendline(sol.encode())
    print(f'proof of work done: {sol}')


def wait_prompt(io) -> None:
    while True:
        chunk = io.recvuntil((PROMPT, b"proof of work:\n"), drop=False)
        print(chunk)
        if chunk.endswith(PROMPT):
            return
        solve_pow(io)


def send_cmd(io, cmd: str) -> None:
    print(f'running command: {cmd}')
    io.sendline(cmd.encode())
    wait_prompt(io)
    print('done')


def upload_binary(io, local_path: Path, remote_path: str) -> None:
    data = local_path.read_bytes()
    blob = base64.b64encode(data).decode()

    print(f"uploading {len(data)} bytes via streamed base64")
    io.sendline(f"head -c {len(blob)} | base64 -d > {remote_path}".encode())
    io.send(blob.encode())
    wait_prompt(io)
    send_cmd(io, f"chmod +x {remote_path}")


def main() -> None:
    parser = argparse.ArgumentParser(description="Upload and run the multifiles solve binary")
    parser.add_argument("host", nargs="?", default=os.environ.get("HOST", "localhost"))
    parser.add_argument("port", nargs="?", type=int, default=int(os.environ.get("PORT", "1337")))
    parser.add_argument(
        "--binary",
        default=str(Path(__file__).with_name("solve")),
        help="Path to the local static solve binary",
    )
    parser.add_argument("--remote-path", default="/tmp/solve", help="Remote path to write and execute")
    parser.add_argument("--ssl", dest="ssl", action="store_true", help="Use TLS when connecting")
    parser.add_argument("--no-ssl", dest="ssl", action="store_false", help="Do not use TLS")
    parser.set_defaults(ssl=False)
    parser.add_argument("--sni", default=None, help="Optional TLS SNI/hostname override")
    args = parser.parse_args()

    local_path = Path(args.binary)
    if not local_path.is_file():
        raise SystemExit(f"binary not found: {local_path}")

    io = remote(args.host, args.port, ssl=args.ssl, sni=args.sni if args.sni else args.host)
    wait_prompt(io)

    send_cmd(io, "stty -echo")
    send_cmd(io, "rm -f /tmp/solve /tmp/solve.b64")
    upload_binary(io, local_path, args.remote_path)

    io.sendline(args.remote_path.encode())
    io.interactive()


if __name__ == "__main__":
    main()
