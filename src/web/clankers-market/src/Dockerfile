FROM python:3.13-slim

RUN apt-get update && apt-get install -y \
    git openssl sqlite3 coreutils gcc libc6-dev \
    --no-install-recommends && \
    rm -rf /var/lib/apt/lists/*

RUN useradd -m web

WORKDIR /app

COPY src/requirements.txt /tmp/requirements.txt
RUN pip install --no-cache-dir -r /tmp/requirements.txt

COPY src/ /app/src
RUN chown -R web:web /app

# Build a SUID helper that prints /flag.txt.
RUN printf '%s\n' \
    '#include <stdio.h>' \
    '#include <stdlib.h>' \
    '#include <unistd.h>' \
    'int main(void) {' \
    '  if (setgid(getegid()) != 0 || setuid(geteuid()) != 0) return 1;' \
    '  FILE *f = fopen("/flag.txt", "r");' \
    '  if (!f) return 1;' \
    '  char buf[512];' \
    '  while (fgets(buf, sizeof(buf), f)) fputs(buf, stdout);' \
    '  fclose(f);' \
    '  return 0;' \
    '}' > /tmp/readflag.c && \
    gcc -O2 -o /usr/local/bin/read-flag /tmp/readflag.c && \
    rm -f /tmp/readflag.c

RUN printf '%s\n' 'bctf{kill_bill_2}' > /flag.txt && \
    chown root:root /flag.txt && \
    chmod 400 /flag.txt

# Only root and group web can execute this SUID binary.
RUN chown root:web /usr/local/bin/read-flag && \
    chmod 4750 /usr/local/bin/read-flag

USER web
RUN git config --global user.email "ctf@ctf.com" && \
    git config --global user.name "CTF" && \
    git config --global core.symlinks false

ENV PYTHONUNBUFFERED=1
EXPOSE 5000
WORKDIR /app/src
CMD ["python3", "app.py"]