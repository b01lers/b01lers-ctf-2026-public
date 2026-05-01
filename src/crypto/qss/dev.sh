#!/bin/sh
sudo sysctl -w kernel.apparmor_restrict_unprivileged_userns=0
sudo docker compose -f deploy/docker-compose.yml down

set -e

if command -v docker >/dev/null 2>&1; then
    runner="sudo docker"
elif command -v podman >/dev/null 2>&1; then
    runner="podman"
else
    echo "Docker/Podman not found"
    exit 1
fi

cd -- "$(dirname -- "$0")/deploy"
$runner compose up -d --build chall
echo '


If you are testing locally:
> ncat localhost 1337'
