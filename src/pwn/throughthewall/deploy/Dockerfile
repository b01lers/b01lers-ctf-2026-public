# ── Stage 1: build QEMU 9.1.0 ────────────────────────────────────────────────
FROM --platform=linux/amd64 debian:bookworm AS qemu-build

RUN apt-get update && apt-get install -y --no-install-recommends \
        wget xz-utils python3 python3-venv \
        build-essential ninja-build pkg-config \
        libglib2.0-dev libfdt-dev libpixman-1-dev \
        zlib1g-dev libslirp-dev \
        busybox-static \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /build
RUN wget -q https://download.qemu.org/qemu-9.1.0.tar.xz \
    && tar xf qemu-9.1.0.tar.xz

RUN cd qemu-9.1.0 \
    && ./configure \
        --target-list=x86_64-softmmu \
        --disable-docs \
        --disable-gtk --disable-sdl --disable-opengl --disable-vnc \
        --disable-werror \
    && make -j$(nproc) \
    && make install

# ── Stage 2: redpwn jail ──────────────────────────────────────────────────────
FROM --platform=linux/amd64 pwn.red/jail

# QEMU binary (make install puts it in /usr/local/bin)
COPY --from=qemu-build /usr/local/bin/qemu-system-x86_64 /srv/usr/bin/qemu-system-x86_64

# QEMU firmware/BIOS data files (required even for -kernel direct boot)
COPY --from=qemu-build /usr/local/share/qemu /srv/usr/local/share/qemu

# Runtime libraries from the build stage
COPY --from=qemu-build /lib/x86_64-linux-gnu       /srv/lib/x86_64-linux-gnu
COPY --from=qemu-build /usr/lib/x86_64-linux-gnu   /srv/usr/lib/x86_64-linux-gnu
COPY --from=qemu-build /lib64                      /srv/lib64

# Static shell for the run script shebang (no dynamic deps)
COPY --from=qemu-build /bin/busybox /srv/bin/busybox
RUN ln -sf busybox /srv/bin/sh

# Challenge files
COPY dist/bzImage                /srv/app/bzImage
COPY build_out/initramfs.cpio.gz /srv/app/initramfs.cpio.gz

# Jail entry point
COPY deploy/run /srv/app/run
RUN  chmod +x /srv/app/run

# Jail settings — JAIL_TIME replaces the explicit `timeout` wrapper
ENV JAIL_TIME=900
ENV JAIL_MEM=512M
ENV JAIL_CPU=100
ENV JAIL_PIDS=50
ENV JAIL_PORT=7352
ENV JAIL_POW=90000

EXPOSE 7352
