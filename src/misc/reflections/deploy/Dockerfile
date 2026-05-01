FROM python:slim AS wrapper-build
RUN apt-get update && apt-get install -y --no-install-recommends gcc libc6-dev && rm -rf /var/lib/apt/lists/*
COPY compiler_wrapper.c /src/compiler_wrapper.c
COPY calc1_compat.c /src/calc1_compat.c
RUN gcc -O2 -s -o /compiler_wrapper /src/compiler_wrapper.c
RUN gcc -O2 -s -o /calc1 /src/calc1_compat.c

FROM python:slim AS app
RUN groupadd -g 1337 testuser && useradd -u 1337 -g 1337 -M -N -r -s /usr/sbin/nologin testuser

FROM pwn.red/jail
COPY --from=app / /srv
COPY server.py /srv/app/run
COPY --from=wrapper-build /calc1 /srv/app/calc1
COPY --from=wrapper-build /compiler_wrapper /srv/app/compiler_wrapper
COPY flag.txt /srv/app/flag.txt
RUN chown 1000:1000 /srv/app/calc1 /srv/app/compiler_wrapper /srv/app/flag.txt
RUN chmod 755 /srv/app/run /srv/app/calc1
RUN chmod 755 /srv/app/compiler_wrapper
RUN chmod 400 /srv/app/flag.txt
ENV JAIL_MEM=128M JAIL_TIME=180 JAIL_PIDS=64 JAIL_TMP_SIZE=10M
