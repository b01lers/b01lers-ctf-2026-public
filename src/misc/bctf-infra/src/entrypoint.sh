#!/bin/bash
set -e

# Create device nodes in the nsjail chroot

mount -t tmpfs -o nosuid,noexec,relatime tmpfs /srv/dev

old_umask=$(umask)
umask 0

for dev in null zero urandom; do
    major=$(stat -c '%t' "/dev/$dev")
    minor=$(stat -c '%T' "/dev/$dev")
    mknod "/srv/dev/$dev" c "0x$major" "0x$minor"
    chmod 666 "/srv/dev/$dev"
done

umask "$old_umask"

mount -o remount,ro,nosuid,noexec,relatime /srv/dev

exec "$@"
