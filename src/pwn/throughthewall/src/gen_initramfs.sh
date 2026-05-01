#!/bin/bash
set -e

# FLAG env var controls the flag placed in the VM (default: fake flag for dist)
FLAG="${FLAG:-bctf{fake_flag}}"

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"

echo "[*] Generating initramfs..."

rm -rf "$SCRIPT_DIR/initramfs"
mkdir -p "$SCRIPT_DIR/initramfs"/{bin,sbin,etc,proc,sys,dev,tmp,home/ctf}
cd "$SCRIPT_DIR/initramfs"

# Compile drop_priv helper (statically linked, drops to uid/gid 1000)
cat > /tmp/drop_priv.c << 'DROPC'
#include <unistd.h>
#include <grp.h>
int main() {
    gid_t g = 1000;
    setgroups(1, &g);
    setgid(1000);
    setuid(1000);
    chdir("/home/ctf");
    execl("/bin/sh", "sh", NULL);
    return 1;
}
DROPC
gcc -static -o bin/drop_priv /tmp/drop_priv.c

# Busybox
BUSYBOX="$(which busybox)"
cp "$BUSYBOX" bin/busybox
chmod 4755 bin/busybox
for cmd in sh ls cat cp mv rm mkdir mount umount chmod chown insmod rmmod lsmod dmesg pwd id su base64 setuidgid; do
    ln -sf busybox bin/$cmd
done

# Init script — FLAG is injected via sed after heredoc creation
cat > init << 'EOF'
#!/bin/sh

mount -t proc none /proc
mount -t sysfs none /sys
mount -t devtmpfs devtmpfs /dev

echo "[*] Loading firewall module..."
if insmod /home/ctf/firewall.ko; then
    echo "[+] Module loaded successfully"
    chmod 666 /dev/firewall
else
    echo "[-] Failed to load module"
fi

echo "PLACEHOLDER_FLAG" > /flag.txt
chmod 400 /flag.txt
chmod 1777 /tmp

echo "root:x:0:0:root:/root:/bin/sh" > /etc/passwd
echo "ctf:x:1000:1000::/home/ctf:/bin/sh" >> /etc/passwd
echo "root:x:0:" > /etc/group
echo "ctf:x:1000:" >> /etc/group

chown -R 1000:1000 /home/ctf

cat << 'BANNER'
=====================================
  Kernel CTF Environment
=====================================
Module:  /home/ctf/firewall.ko
Exploit: /home/ctf/exploit
Flag:    /flag.txt (root only)

You are: ctf (uid=1000)
Goal: Exploit the firewall module!
=====================================
BANNER

cd /home/ctf
while true; do
    /bin/drop_priv
done

poweroff -f
EOF

# Inject real flag
sed -i "s|PLACEHOLDER_FLAG|${FLAG}|" init
chmod +x init

# Device nodes (needs root or fakeroot)
mknod -m 666 dev/null    c 1 3
mknod -m 666 dev/zero    c 1 5
mknod -m 666 dev/tty     c 5 0
mknod -m 666 dev/console c 5 1
mknod -m 666 dev/random  c 1 8
mknod -m 666 dev/urandom c 1 9

# Copy kernel module
if [ -f "$SCRIPT_DIR/firewall.ko" ]; then
    cp "$SCRIPT_DIR/firewall.ko" home/ctf/
    echo "[+] Copied firewall.ko"
else
    echo "[-] Warning: firewall.ko not found at $SCRIPT_DIR/firewall.ko"
fi

# Pack
find . | cpio -o -H newc -R root:root | gzip > "$SCRIPT_DIR/initramfs.cpio.gz"

cd "$SCRIPT_DIR"
echo "[+] initramfs.cpio.gz created ($(du -h initramfs.cpio.gz | cut -f1))"
