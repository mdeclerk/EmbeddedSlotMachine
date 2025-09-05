#!/bin/sh
set -e

docker run --rm -v $(PWD):/project buildenv:aarch64 make uki
docker run --rm -v $(PWD):/project buildenv:x86_64 make uki
docker run --rm -v $(PWD):/project buildenv:host-arch make isoimage