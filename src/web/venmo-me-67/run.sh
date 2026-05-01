#!/bin/sh
set -e
cd deploy
sudo docker build . -t 'venmo-me-67'
sudo -E docker push 'localhost:5000/venmo-me-67'
kubectl create -f challenge.yml
