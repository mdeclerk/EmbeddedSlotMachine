#!/bin/sh
set -e

getvar() { make -s -f config.mk print-"$1"; }

EFI_CODE=$(getvar AAVMF_CODE)
EFI_VARS=$(getvar AAVMF_VARS)
ISO=$(getvar ISO)

qemu-system-aarch64 \
    -M virt -cpu max \
    -drive if=pflash,format=raw,unit=0,readonly=on,file="$EFI_CODE" \
    -drive if=pflash,format=raw,unit=1,file="$EFI_VARS" \
    -cdrom "$ISO" -boot d \
    -device qemu-xhci -device usb-kbd \
    -device ramfb -display cocoa \
    -serial mon:stdio