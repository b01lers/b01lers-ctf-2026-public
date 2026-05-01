#!/bin/bash

rm -r ../dist
mkdir ../dist

cp chall.py ../dist/chall.py
echo "bctf{fake_flag}" > ../dist/flag.txt
python chall.py > ../dist/output.txt

cd ../dist
zip -r dist .
rm *.py *.txt
