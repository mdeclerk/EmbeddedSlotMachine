#!/bin/sh
docker run --rm -it -v $(PWD):/project -w /project buildenv:aarch64 bash