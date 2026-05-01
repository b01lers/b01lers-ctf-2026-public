FROM python:3.13-slim-bookworm AS builder

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

COPY --from=builder /bin /srv/bin
COPY --from=builder /lib /srv/lib
COPY --from=builder /lib64 /srv/lib64
COPY --from=builder /usr /srv/usr
COPY --from=builder /etc/ld.so.cache /srv/etc/ld.so.cache

COPY --from=builder /usr/local/rustup /srv/usr/local/rustup
COPY --from=builder /usr/local/cargo /srv/usr/local/cargo

COPY ./chall.py /srv/app/chall.py
COPY ./flag.txt /flag.txt

RUN ln -sf usr/lib32 /srv/lib32 && \
    ln -sf usr/lib64 /srv/lib64 && \
    ln -sf gcc-12 /srv/usr/bin/cc && \
    ln -sf gcc-12 /srv/usr/bin/gcc

RUN mkdir -p /srv/tmp /srv/dev

RUN printf '#!/bin/sh\necho "$FLAG" > /tmp/.flag\nexec env -i \\\n  PATH=/usr/local/bin:/usr/bin:/bin \\\n  FLAG_PATH=/tmp/.flag \\\n  /usr/local/bin/python3 /app/chall.py\n' > /srv/app/run && \
    chmod +x /srv/app/run

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
