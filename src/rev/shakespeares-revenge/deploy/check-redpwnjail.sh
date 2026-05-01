#!/bin/sh
set -e

fail=0

if [ -r /proc/sys/kernel/unprivileged_userns_clone ]; then
    userns_clone="$(cat /proc/sys/kernel/unprivileged_userns_clone)"
    if [ "$userns_clone" = "0" ]; then
        echo "kernel.unprivileged_userns_clone is disabled (0)." >&2
        fail=1
    fi
fi

if [ -r /proc/sys/kernel/apparmor_restrict_unprivileged_userns ]; then
    apparmor_userns="$(cat /proc/sys/kernel/apparmor_restrict_unprivileged_userns)"
    if [ "$apparmor_userns" = "1" ]; then
        echo "kernel.apparmor_restrict_unprivileged_userns is enabled (1)." >&2
        fail=1
    fi
fi

if [ "$fail" -ne 0 ]; then
    echo "redpwn/jail prerequisites are not satisfied on this host." >&2
    echo "Apply these settings as root, then retry:" >&2
    echo "  sysctl -w kernel.unprivileged_userns_clone=1" >&2
    echo "  sysctl -w kernel.apparmor_restrict_unprivileged_userns=0" >&2
    exit 1
fi

echo "Host namespace settings look compatible with redpwn/jail."
