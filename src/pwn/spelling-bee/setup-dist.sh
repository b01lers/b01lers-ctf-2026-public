#!/bin/sh
rm -rf ./dist
mkdir -p dist/spelling-bee

cp ./build_out/* ./dist/spelling-bee
cp ./src/forth.c ./dist/spelling-bee

zip -r ./dist/spelling-bee.zip ./dist/spelling-bee
rm -r ./dist/spelling-bee
