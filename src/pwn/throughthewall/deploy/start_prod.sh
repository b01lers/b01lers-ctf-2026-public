#!/bin/bash
# Production QEMU launch — no debug port (-s), 5-minute timeout

exec qemu-system-x86_64 \
    -m 256M \
    -nographic \
    -kernel /app/bzImage \
    -append "console=ttyS0 loglevel=3 oops=panic panic=-1 pti=on kaslr" \
    -no-reboot \
    -cpu qemu64,+smep,+smap \
    -smp 2 \
    -initrd /app/initramfs.cpio.gz \
    -monitor /dev/null
