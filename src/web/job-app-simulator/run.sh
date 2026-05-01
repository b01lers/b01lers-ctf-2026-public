#!/bin/sh
set -e
cd deploy
sudo docker build . -t 'cgi'
sudo -E docker push 'localhost:5000/cgi'
kubectl create -f challenge.yml
