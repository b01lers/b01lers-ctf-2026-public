#!/bin/bash
chall_name=job-app-simulator

shopt -s extglob
script_dir="$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" > /dev/null 2>&1 && pwd )"

rm -f "../dist/$chall_name.zip"

folder=$(mktemp -d)
trap 'rm -rf "$folder"' EXIT
mkdir -p "$folder/$chall_name"

cp -r !(make_dist.sh|venv|flag.txt) "$folder/$chall_name"
echo "bctf{fake_flag}" > "$folder/$chall_name/flag.txt"
cd "$folder" || exit
zip -r "$chall_name.zip" "$chall_name"
cp "$chall_name.zip" "$script_dir/../dist/"