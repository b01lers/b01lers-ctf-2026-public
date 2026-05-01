#!/bin/sh

cd "$(dirname $0)"

# nsjail wipes path
export PATH="/rustup_home/toolchains/nightly-2025-09-25-x86_64-unknown-linux-gnu/bin/:$PATH"

# make cargo home for storing cargo downloads
mkdir -p cargo_home
export CARGO_HOME="$PWD/cargo_home"

# rustup home is already setup
export RUSTUP_HOME="/rustup_home"

mkdir -p src

python3 get_payload.py || exit 1

cargo run
