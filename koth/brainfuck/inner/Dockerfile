FROM alpine:edge AS builder
RUN apk add --no-cache build-base git musl-dev
RUN git clone https://github.com/primo-ppcg/bfci.git /bfci
WORKDIR /bfci
RUN cc -Wall -Wextra -O3 -s -static -o bfci bfci.c src/*.c

FROM python:3.14.3-alpine AS python
RUN pip install zstd

FROM pwn.red/jail
COPY --from=python / /srv
COPY --from=builder /bfci/bfci /srv/usr/local/bin/bfci
COPY run.py /srv/app/run
RUN chmod +x /srv/app/run

ENV JAIL_TIME=120 JAIL_MEM=256M JAIL_PIDS=10 JAIL_PORT=8990 JAIL_TMP_SIZE=32M JAIL_CPU=750
