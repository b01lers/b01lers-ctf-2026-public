FROM --platform=linux/amd64 debian:bookworm-slim

RUN apt-get update && \
    DEBIAN_FRONTEND=noninteractive apt-get install -y \
        qemu-system-x86 \
        socat && \
    rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY ./build_out/bzImage /app/bzImage
COPY ./build_out/initrd.cpio.gz /app/initrd.cpio.gz
COPY ./deploy/wrapper.sh /app/start.sh

RUN chmod +x /app/start.sh

EXPOSE 6767

CMD ["socat", "TCP-LISTEN:6767,reuseaddr,fork", "EXEC:/app/start.sh,pty,setsid,sigint,sane"]
