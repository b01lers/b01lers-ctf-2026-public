FROM alpine:3.23.3 AS builder

RUN apk add --no-cache build-base git python3
# latest stable release at the time of developing this challenge
RUN git clone --depth 1 --branch v1.27.0 https://github.com/micropython/micropython.git /opt/micropython
WORKDIR /opt/micropython/ports/unix

# patch out os ;)
RUN sed -i 's/#define MICROPY_PY_OS (1)/\/\/ #define MICROPY_PY_OS (1)/' \
        ./variants/minimal/mpconfigvariant.h

RUN make -j4 submodules
RUN make -j4 VARIANT=minimal

# create catflag
WORKDIR /opt
COPY catflag.c .
RUN gcc -o catflag catflag.c

FROM alpine:3.23.3 AS app
COPY --from=builder /opt/micropython/ports/unix/build-minimal/micropython /usr/local/bin/micropython
COPY --from=builder /opt/catflag /catflag

RUN mkdir -p /app
RUN echo -ne "#!/bin/sh\nexec /usr/local/bin/micropython -i" > /app/run

RUN chmod 755 /app/run && \
    chmod 111 /catflag

FROM pwn.red/jail
COPY --from=app / /srv

ENV JAIL_TIME=120