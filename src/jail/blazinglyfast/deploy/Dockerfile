FROM python:3.13-slim-bookworm AS builder

# Install 32-bit libs and Rust toolchain
RUN dpkg --add-architecture i386 && \
    apt-get update && \
    apt-get install -y --no-install-recommends \
        curl ca-certificates \
        libc6:i386 libgcc-s1:i386 libc6-dev-i386 \
        gcc gcc-multilib \
    && rm -rf /var/lib/apt/lists/*

ENV RUSTUP_HOME=/usr/local/rustup \
    CARGO_HOME=/usr/local/cargo \
    PATH="/usr/local/cargo/bin:${PATH}"

RUN curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | \
    sh -s -- -y --default-toolchain stable --profile minimal && \
    rustup target add i686-unknown-linux-gnu

FROM ghcr.io/redpwn/jail

# Copy system base from builder (shell, libs, linker)
COPY --from=builder /bin /srv/bin
COPY --from=builder /lib /srv/lib
COPY --from=builder /lib64 /srv/lib64
COPY --from=builder /usr /srv/usr
COPY --from=builder /etc/ld.so.cache /srv/etc/ld.so.cache

# Copy Rust toolchain from builder
COPY --from=builder /usr/local/rustup /srv/usr/local/rustup
COPY --from=builder /usr/local/cargo /srv/usr/local/cargo

# Copy challenge script (flag stays outside /srv — not in the jailed filesystem)
COPY ./src/chall.py /srv/app/chall.py
COPY ./flag.txt /flag.txt

# Create symlinks matching Debian filesystem layout
RUN ln -sf usr/lib32 /srv/lib32 && \
    ln -sf usr/lib64 /srv/lib64 && \
    ln -sf gcc-12 /srv/usr/bin/cc && \
    ln -sf gcc-12 /srv/usr/bin/gcc

# Pre-create mount points inside srv (nsjail mounts /srv as read-only root)
# /proc intentionally omitted — rustc is called directly with --sysroot to
# avoid needing /proc/self/exe, which also prevents binaries from reading
# parent process memory via /proc/<pid>/mem.
RUN mkdir -p /srv/tmp /srv/dev

# Per-connection run script:
#   1. Writes FLAG to a tmp file (writable tmpfs)
#   2. Execs Python with a clean env (no FLAG) so /proc/1/environ is clean
#   Python reads /tmp/.flag at startup, deletes it, then handles the connection.
RUN printf '#!/bin/sh\necho "$FLAG" > /tmp/.flag\nexec env -i \\\n  PATH=/usr/local/bin:/usr/bin:/bin \\\n  FLAG_PATH=/tmp/.flag \\\n  /usr/local/bin/python3 /app/chall.py\n' > /srv/app/run && \
    chmod +x /srv/app/run

# Entrypoint: reads flag from outside /srv, exports as JAIL_ENV_FLAG,
# then starts the jail. The flag never exists as a file inside /srv.
RUN printf '#!/bin/sh\nexport JAIL_ENV_FLAG="$(cat /flag.txt)"\nexec /jail/run\n' > /entrypoint.sh && \
    chmod +x /entrypoint.sh
ENTRYPOINT ["/bin/sh", "/entrypoint.sh"]

ENV JAIL_TIME=60 \
    JAIL_MEM=512M \
    JAIL_PIDS=32 \
    JAIL_CPU=1000 \
    JAIL_PORT=1337 \
    JAIL_TMP_SIZE=67108864 \
    JAIL_DEV=null,zero,urandom
