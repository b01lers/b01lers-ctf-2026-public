#!/bin/bash

rm -r ../dist
mkdir ../dist

cp ../build_out/* ../dist
cp chall.c ../dist
cp Dockerfile ../dist
echo "bctf{fake_flag}" > ../dist/flag.txt

cd ../dist
zip -r dist .
rm chall Dockerfile *.so* *.c *.txt
