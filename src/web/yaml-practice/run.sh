#!/bin/sh
set -e
cd deploy
sudo docker build . -t 'yaml-practice'
sudo -E docker push 'localhost:5000/yaml-practice'
kubectl create -f challenge.yml
