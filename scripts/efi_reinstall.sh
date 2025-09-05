#!/bin/sh
docker run --rm -v $(PWD):/project buildenv:host-arch make efi-reinstall