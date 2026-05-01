#!/bin/sh
set -e

cd "$(dirname "$0")"

# Build kernel module if headers are available; otherwise use pre-compiled firewall.ko
if [ -n "$KDIR" ] && [ -d "$KDIR" ]; then
    echo "[*] Building firewall.ko against $KDIR"
    make KDIR="$KDIR"
elif [ -d "/lib/modules/$(uname -r)/build" ]; then
    echo "[*] Building firewall.ko"
    make
else
    echo "[!] No kernel headers found — using pre-compiled firewall.ko"
fi

# Generate initramfs with fake flag for dist
echo "[*] Generating initramfs (fake flag)..."
bash gen_initramfs.sh
