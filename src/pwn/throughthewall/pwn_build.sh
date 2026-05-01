#!/bin/sh
set -e

cd "$(dirname "$0")"

mkdir -p build_out

if command -v docker >/dev/null 2>&1; then
    runner="docker"
elif command -v podman >/dev/null 2>&1; then
    runner="podman"
else
    echo "Docker/Podman not found"
    exit 1
fi

export CHALL_HASH='throughthewall'

# Build the initramfs (with real flag) inside a container
cd deploy && $runner compose up --build build
