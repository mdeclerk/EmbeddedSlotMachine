#!/bin/sh
docker run --rm -v $(PWD):/project -w /project buildenv:host-arch make efi-reinstall