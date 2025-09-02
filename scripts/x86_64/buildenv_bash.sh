#!/bin/sh
docker run --rm -it -v $(PWD):/project -w /project buildenv:x86_64 bash