#!/bin/sh

set -eu

gdb_args=""

while [ "$#" -gt 0 ]; do
    case "$1" in
        --gdb-port)
            if [ "$#" -lt 2 ]; then
                echo "missing value for --gdb-port" >&2
                exit 1
            fi
            gdb_args="--gdb-port $2"
            shift 2
            ;;
        -g)
            if [ "$#" -lt 2 ]; then
                echo "missing value for -g" >&2
                exit 1
            fi
            gdb_args="--gdb-port $2"
            shift 2
            ;;
        *)
            echo "usage: $0 [--gdb-port PORT]" >&2
            exit 1
            ;;
    esac
done

./build.sh

testvm run bzImage --initrd initrd.cpio.gz --arch x86_64 --memory 256M --append nokaslr --run-host-path solve $gdb_args
