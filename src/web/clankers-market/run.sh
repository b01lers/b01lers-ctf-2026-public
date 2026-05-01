#!/bin/sh
set -e
cd deploy
sudo docker build . -t 'clankers-market'
sudo -E docker push 'localhost:5000/clankers-market'
kubectl create -f challenge.yml
