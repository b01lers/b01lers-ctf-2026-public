FROM python:3.14-slim

WORKDIR /app

ENV PYTHONDONTWRITEBYTECODE=1
ENV PYTHONUNBUFFERED=1

RUN pip install --no-cache-dir starlette uvicorn jinja2 itsdangerous

RUN mkdir -p /app/src

COPY src/src/main.py /app/src/main.py
COPY src/src/hatchery.py /app/src/hatchery.py
COPY src/src/palette.py /app/src/palette.py
COPY src/src/db.py /app/src/db.py
COPY src/asciiart /app/asciiart
COPY src/templates /app/templates

RUN mkdir -p /app/incubator \
    && useradd -r -s /usr/sbin/nologin appuser \
    && chown -R root:root /app \
    && chmod 555 /app \
    && chmod 444 /app/src/main.py /app/src/hatchery.py /app/src/palette.py /app/src/db.py /app/asciiart \
    && find /app/templates -type d -exec chmod 555 {} \; \
    && find /app/templates -type f -exec chmod 444 {} \; \
    && chown appuser:appuser /app/incubator \
    && chmod 700 /app/incubator

USER appuser

EXPOSE 8000

ENTRYPOINT [ "python3", "/app/src/main.py", "bctf{you_w1n!1}" ]
