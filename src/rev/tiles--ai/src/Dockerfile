FROM alpine:3.23
RUN apk add --no-cache clang
CMD ["clang", \
        "-s", "-flto", "-static", "-O2", "-march=sapphirerapids", \
        "-fno-exceptions", "-fno-unwind-tables", "-fno-asynchronous-unwind-tables", \
        "-masm=intel", \
        "/src/main.c", "-o", "/build/chall"]
