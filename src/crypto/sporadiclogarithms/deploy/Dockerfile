FROM sagemath/sagemath:latest AS base

USER root

ENV FLAG="bctf{th3_m0n5t3r_gr0up_w45_t0_1mpr4ct1c4l_:(}"

RUN apt-get update && apt-get install -y socat && rm -rf /var/lib/apt/lists/*

RUN mkdir -p /app/tmp && chmod 777 /app/tmp && \
    mkdir -p /app && chmod 755 /app

COPY --chmod=755 ./deploy/wrapper.sh /app/run
COPY --chmod=755 ./dist/chall.py /app/chall.py

WORKDIR /app

CMD ["socat", "-T60", "TCP-LISTEN:8888,reuseaddr,fork", "EXEC:/app/run"]