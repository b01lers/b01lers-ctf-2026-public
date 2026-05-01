# throughthewall — Deployment Notes

## File Overview

| File | Purpose |
|---|---|
| `src/firewall.c` | Vulnerable kernel module source (UAF via `fw_del_rule` not clearing the pointer) |
| `src/firewall.ko` | Pre-compiled module (kernel 5.15.167) — packed into initramfs at build time |
| `src/Makefile` | Out-of-tree kernel module build (`make KDIR=/path/to/headers`) |
| `src/gen_initramfs.sh` | Builds the initramfs cpio archive; reads `$FLAG` env var and injects it via `sed` into the init script |
| `src/build.sh` | Local build helper: compiles module (if kernel headers available) then runs `gen_initramfs.sh` with fake flag |
| `deploy/Dockerfile` | Production image: multi-stage — builds QEMU 9.1.0 from source, then wraps challenge in `pwn.red/jail` (nsjail) |
| `deploy/Dockerfile_build` | Build image: installs `busybox-static`, `gcc`, `cpio`, `gzip`, `fakeroot`; runs `gen_initramfs.sh` with the real flag |
| `deploy/run` | Jail entry point — QEMU launch args executed by nsjail for each connection (replaces `start_prod.sh` + socat) |
| `deploy/docker-compose.yml` | Defines `chall` (serves challenge via redpwn jail on port 7352) and `build` (generates initramfs) services |
| `dist/bzImage` | Kernel image (5.15.167) — given to competitors and used in production |
| `dist/initramfs.cpio.gz` | Initramfs with **fake flag** — given to competitors for local testing |
| `dist/start.sh` | QEMU launch script for competitors (includes `-s` on port 1234 for local GDB) |
| `pwn_build.sh` | Runs the `build` Docker service to produce `build_out/initramfs.cpio.gz` with the real flag |

## Deploy Flow

```
# Step 1: build initramfs with real flag
./pwn_build.sh
  └─ docker compose up --build build
       └─ Dockerfile_build
            fakeroot gen_initramfs.sh   (FLAG = contents of flag.txt)
            → build_out/initramfs.cpio.gz

# Step 2: start challenge container
docker compose -f deploy/docker-compose.yml up --build chall
  └─ Dockerfile  (multi-stage)
       ├─ stage qemu-build: compiles QEMU 9.1.0 from source
       └─ stage pwn.red/jail:
            COPY dist/bzImage
            COPY build_out/initramfs.cpio.gz
            COPY deploy/run  →  /srv/app/run
            nsjail listens on :7352, spawns /app/run per connection
```

Step 1 must run before Step 2. The `chall` image copies from `build_out/` at build time.

To rebuild everything after any change:
```
docker compose -f deploy/docker-compose.yml up --build build && \
docker compose -f deploy/docker-compose.yml up --build --force-recreate chall
```

## Jail (redpwn jail / nsjail)

The challenge is wrapped with `pwn.red/jail` instead of bare socat. For each TCP connection nsjail:
- pivot-roots into `/srv` (read-only)
- runs `/app/run` (→ `deploy/run`) as uid 1000
- enforces: `JAIL_TIME=300s`, `JAIL_MEM=512M`, `JAIL_PIDS=50`, `JAIL_CPU=100%`

The `chall` service requires `privileged: true` in docker-compose for nsjail's cgroup/namespace setup.

QEMU runs without KVM (TCG software emulation). Expect ~60 s boot time per connection.

## Flag Injection

The flag is **not** baked into `dist/initramfs.cpio.gz`. It is injected at Docker build time:

1. `Dockerfile_build` reads `flag.txt` and passes it as `$FLAG` to `gen_initramfs.sh`
2. `gen_initramfs.sh` writes `PLACEHOLDER_FLAG` into the init script, then replaces it with `sed`
3. Inside the VM, `/flag.txt` is created at boot by the init script, owned by root, mode 400

## Kernel Mitigations (QEMU flags)

| Mitigation | Flag |
|---|---|
| KASLR | `-append "... kaslr"` |
| SMEP | `-cpu qemu64,+smep` |
| SMAP | `-cpu qemu64,+smap` |
| PTI (KPTI) | `-append "... pti=on"` |

The debug port (`-s`, GDB on 1234) is **only** in `dist/start.sh` for competitor local use. `deploy/run` omits it.

## Competitor Distribution

Bundle `dist/` as a zip:
```
throughthewall.zip
  bzImage
  initramfs.cpio.gz   ← fake flag inside
  start.sh            ← run locally with: bash start.sh
```
