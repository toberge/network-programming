#!/usr/bin/env bash

sudo docker build -t executor .
sudo docker container run --rm executor python -c 'print("Testing, testing...")'
