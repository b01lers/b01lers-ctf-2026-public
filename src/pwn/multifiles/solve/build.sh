#!/bin/sh

nix-shell -p glibc.static --run "gcc -g -static -o solve solve.c"
