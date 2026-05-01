#!/bin/sh
set -e

if command -v docker >/dev/null 2>&1; then
    runner="sudo -E docker"
elif command -v podman >/dev/null 2>&1; then
    runner="podman"
else
    echo "Docker/Podman not found"
    exit 1
fi

cd "$(dirname $0)"

mkdir -p build_out

# pass user id and group id we want chall build file to have to docker-compose
export USER_ID=$(id -u)
export GROUP_ID=$(id -g)
export CHALL_HASH='slopstore' # please include this envar in your final build

cd deploy
$runner compose up --build build
