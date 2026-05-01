#!/bin/sh
set -e
cd deploy
sudo docker build . -t 'egg'
sudo -E docker push 'localhost:5000/egg'
kubectl create -f challenge.yml
