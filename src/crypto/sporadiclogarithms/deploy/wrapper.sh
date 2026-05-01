#!/bin/sh

export HOME=/app
export TMPDIR=/app/tmp
export TMP=/app/tmp
export TEMP=/app/tmp

exec /usr/bin/sage -python /app/chall.py