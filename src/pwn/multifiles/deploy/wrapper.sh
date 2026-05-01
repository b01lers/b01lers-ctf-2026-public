#!/bin/sh
set -eu

exec timeout 300 qemu-system-x86_64 \
    -nodefaults \
    -m 256M \
    -nographic \
    -kernel /app/bzImage \
    -initrd /app/initrd.cpio.gz \
    -append "console=ttyS0 loglevel=3 oops=panic panic=-1 pti=on kaslr" \
    -cpu qemu64,+smep,+smap \
    -smp 1 \
    -no-reboot \
    -serial stdio \
    -monitor none
