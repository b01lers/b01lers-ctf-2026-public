FROM python:3.14.4-slim AS app

COPY chal.py /app/run
COPY flag.txt /flag.txt
RUN chmod 755 /app/run && \
    chmod 444 /flag.txt && \    
    mv /flag.txt "/flag-$(cat /dev/urandom | tr -cd 'a-f0-9' | head -c 32).txt"

FROM pwn.red/jail
COPY --from=app / /srv