#!/bin/sh
set -e

getvar() { make -s -f config.mk print-"$1"; }

EFI_CODE=$(getvar OVMF_CODE)
EFI_VARS=$(getvar OVMF_VARS)
ISO=$(getvar ISO)

qemu-system-x86_64 \
    -drive if=pflash,format=raw,unit=0,readonly=on,file="$EFI_CODE" \
    -drive if=pflash,format=raw,unit=1,file="$EFI_VARS" \
    -cdrom "$ISO" -boot d \
    -device ramfb -vga none -display cocoa \
    -serial mon:stdio