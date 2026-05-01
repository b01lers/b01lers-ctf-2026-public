FROM python:3.14-slim

ENV PYTHONDONTWRITEBYTECODE=1
ENV PYTHONUNBUFFERED=1

WORKDIR /app

RUN apt-get update \
&& apt-get install -y gcc libc6-dev curl \
&& rm -rf /var/lib/apt/lists/*

COPY ./src/main.py /app/main.py
COPY ./src/templates /app/templates
COPY ./src/readflag.c /tmp/readflag.c
COPY ./flag.txt /flag

RUN mkdir -p /app/uploads \
    && mkdir -p /app/.venv \
    && chown root:root /flag \
    && chmod 0400 /flag \
    && gcc /tmp/readflag.c -O2 -o /readflag \
    && chown root:root /readflag \
    && chmod 4755 /readflag \
    && rm -f /tmp/readflag.c \
    && useradd -m ctf \
    && chown ctf /app/uploads \
    && chown ctf /app/.venv

USER ctf

RUN python3 -m venv --system-site-packages /app/.venv \
    && /app/.venv/bin/pip install --no-cache-dir \
        starlette \
        uvicorn \
        jinja2 \
        python-multipart \
        prefect

EXPOSE 8000

CMD ["/app/.venv/bin/python", "main.py"]
