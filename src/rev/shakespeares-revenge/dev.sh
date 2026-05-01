#!/bin/sh
set -e

choose_runner() {
    if command -v docker >/dev/null 2>&1; then
        if docker info >/dev/null 2>&1; then
            if docker info --format '{{json .SecurityOptions}}' 2>/dev/null | grep -qi rootless; then
                echo "Rootless Docker detected. redpwn/jail needs rootful Docker." >&2
                echo "Use sudo docker or switch to a rootful Docker daemon." >&2
                exit 1
            fi
            echo "docker"
            return
        fi

        # Fall back to sudo if the current user cannot access the daemon.
        echo "sudo docker"
        return
    fi

    echo "Docker not found" >&2
    exit 1
}

runner="$(choose_runner)"

cd -- "$(dirname -- "$0")/deploy"
./check-redpwnjail.sh
$runner compose up -d --build chall
echo '


If you are testing locally:
> ncat localhost 1337'
