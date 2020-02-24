#!/usr/bin/env bash

# Build from .
# This expects a Dockerfile in the current directory 
sudo docker build -t executor .
# Run stuff
sudo docker container run --rm executor python -c 'print("Testing, testing...")'
