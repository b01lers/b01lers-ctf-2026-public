# Builder stages

# Fish / *><> (go-starfish)
FROM golang:1.22-alpine AS fish-builder
RUN apk add --no-cache git
RUN CGO_ENABLED=0 go install github.com/redstarcoder/go-starfish/cmd/starfish@latest

# Whitespace JIT
FROM alpine:3.21 AS whitespace-builder
RUN apk add --no-cache build-base git
RUN git clone https://github.com/drebelsky/whitespace-jit.git /build
WORKDIR /build
RUN sed -i '1i #include <cstddef>' datatypes.h && \
    make CXXFLAGS="-O3 -Wall -Wpedantic -Werror -std=c++17 -static"

# Python 0.9.1
FROM i386/alpine:3.21 AS python0-builder
RUN apk add --no-cache build-base git
RUN git clone --depth 1 --filter=blob:none --sparse \
    https://github.com/di/vintage-python.git /repo && \
    cd /repo && git sparse-checkout set 0.9.1/python-0.9.1 && \
    mv 0.9.1/python-0.9.1 /build
WORKDIR /build/src/
RUN make SYSVDEF=-DSYSV CFLAGS="-static -std=gnu89 -w"

# Final image
FROM alpine:edge

RUN apk add --no-cache \
    bash \
    tcc \
    tcc-dev \
    tcc-libs-static \
    musl-dev \
    erlang27 \
    elixir \
    lua5.4 \
    perl \
    ruby \
    guile \
    openjdk25-jdk \
    rust \
    ghc \
    cabal \
    gcompat \
    gmp \
    wget \
    python3 \
    py3-pip \
    zig

# Odin is only in edge/testing
RUN echo "https://dl-cdn.alpinelinux.org/alpine/edge/testing" >> /etc/apk/repositories && \
    apk add --no-cache odin

# J Language (glibc binary via gcompat)
RUN wget -q https://www.jsoftware.com/download/j9.6/install/j9.6_linux64.tar.gz -O /tmp/j.tar.gz && \
    tar xzf /tmp/j.tar.gz -C /opt/ && rm /tmp/j.tar.gz && \
    ln -s /opt/j9.6/bin/jconsole /usr/local/bin/jconsole && \
    ln -s /opt/j9.6/bin/libj.so /opt/j9.6/bin/libj.so.9.6

# GolfScript (ruby script)
RUN wget -O /usr/local/bin/golfscript.rb \
    https://raw.githubusercontent.com/darrenks/golfscript/refs/heads/main/golfscript.rb && \
    printf '#!/bin/sh\nruby /usr/local/bin/golfscript.rb "$@"\n' > /usr/local/bin/golfscript && \
    chmod +x /usr/local/bin/golfscript

# Julia
COPY --from=julia:1.11-alpine /usr/local/julia/ /usr/local/julia/
RUN ln -s /usr/local/julia/bin/julia /usr/local/bin/julia

# Bun (TypeScript runtime)
COPY --from=oven/bun:alpine /usr/local/bin/bun /usr/local/bin/bun

# Static binaries from builder stages
COPY --from=fish-builder       /go/bin/starfish       /usr/local/bin/starfish
RUN printf '#!/bin/sh\nstarfish "$@" 2>&1\n' > /usr/local/bin/fish && chmod +x /usr/local/bin/fish
COPY --from=whitespace-builder /build/compile         /usr/local/bin/whitespace
COPY --from=python0-builder    /build/src/python      /usr/local/bin/python0
COPY --from=python0-builder    /build/lib/             /usr/local/lib/python0/

WORKDIR /app
COPY languages.toml /app/languages.toml
COPY runner.py /app/runner.py
RUN chmod +x /app/runner.py

WORKDIR /work
ENTRYPOINT ["/app/runner.py"]
