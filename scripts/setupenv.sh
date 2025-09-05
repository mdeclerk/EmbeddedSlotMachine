#!/bin/sh
set -e

docker build --platform=linux/aarch64 -t buildenv:aarch64 .
docker build --platform=linux/x86_64 -t buildenv:x86_64 .
docker build -t buildenv:host-arch .