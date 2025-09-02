#!/bin/sh
set -e

docker buildx build --platform=linux/aarch64 -t buildenv:aarch64 .
docker buildx build --platform=linux/x86_64 -t buildenv:x86_64 .
docker buildx build -t buildenv:host-arch .