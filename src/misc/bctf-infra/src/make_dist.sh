#!/bin/bash
chall_name=bctf-infra

shopt -s extglob
script_dir="$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" > /dev/null 2>&1 && pwd )"

rm -f "../dist/$chall_name.zip"

folder=$(mktemp -d)
trap 'rm -rf "$folder"' EXIT
mkdir -p "$folder/$chall_name"

cp -r !(make_dist.sh|venv) "$folder/$chall_name"
echo "bctf{real_flag_will_be_here}" > "$folder/$chall_name/app/chals/chal3/flag.txt"
cd "$folder" || exit
zip -r "$chall_name.zip" "$chall_name"
cp "$chall_name.zip" "$script_dir/../dist/"