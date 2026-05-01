#!/bin/sh
set -e

/app/challenge_server.py &
server_pid="$!"

trap 'kill "$server_pid" 2>/dev/null || true' EXIT INT TERM

sleep 0.5

nc 127.0.0.1 1337