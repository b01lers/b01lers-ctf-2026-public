#!/usr/bin/env python
import grp
import os
import pwd
import shutil
import socket
import subprocess
from multiprocessing import Process
from pathlib import Path

CHAL_FOLDER = Path(__file__).parent / "chals"

CHALLENGES = [f.name for f in CHAL_FOLDER.iterdir() if f.is_dir() and not f.is_symlink()]


def run_challenge(challenge: str, socket: socket.socket):
    with socket:
        uid = pwd.getpwnam(challenge).pw_uid
        gid = grp.getgrnam(challenge).gr_gid
        os.setgid(gid)
        os.setuid(uid)

        root = Path("/tmp/" + os.urandom(8).hex())
        root.mkdir()
        try:
            shutil.copytree(CHAL_FOLDER / challenge, root / "app")

            subprocess.run(
                [root / "app/chal.py"],
                cwd=root / "app",
                env={"PATH": "/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin"},
                stdin=socket.fileno(),
                stdout=socket.fileno(),
                stderr=socket.fileno(),
            )
        finally:
            shutil.rmtree(root)


def main():
    server = socket.create_server(("0.0.0.0", 1337))

    processes: set[Process] = set()
    while True:
        try:
            for process in list(processes):
                if not process.is_alive():
                    process.close()
                    processes.remove(process)

            client, _ = server.accept()

            with client:
                client.sendall(f"Challenges:\n{'\n'.join(CHALLENGES)}\n> ".encode())

                buf = b""
                while True:
                    data = client.recv(1024)
                    if not data:
                        break
                    if b"\n" in data:
                        buf += data[: data.index(b"\n")]
                        break
                    buf += data

                if not buf:
                    client.close()
                    continue

                challenge = buf.strip().decode(errors="strict")

                if challenge not in CHALLENGES:
                    client.sendall(b"Invalid Challenge\n")
                    client.close()
                    continue

                client.sendall(f"{challenge}:\n".encode())

                proc = Process(target=run_challenge, args=(challenge, client), daemon=True)
                proc.start()
                processes.add(proc)

        except Exception:
            pass


if __name__ == "__main__":
    main()
