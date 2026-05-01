#!/bin/bash
# start.sh

qemu-system-x86_64 \
    -m 256M \
    -nographic \
    -kernel ./bzImage \
    -append "console=ttyS0 loglevel=3 oops=panic panic=-1 pti=on kaslr" \
    -no-reboot \
    -cpu qemu64,+smep,+smap \
    -smp 2 \
    -initrd ./initramfs.cpio.gz \
    -monitor /dev/null \
    -s \
    2>&1 | tee vm.log