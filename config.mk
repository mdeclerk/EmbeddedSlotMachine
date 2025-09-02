ARCH 			?= $(shell uname -m)

OUT 			?= out
UKI_AARCH64 	?= $(OUT)/uki-aarch64
UKI_X86_64 		?= $(OUT)/uki-x86_64
ISO 			?= $(OUT)/cdrom.iso

# if 1 then simple helloworld will be bundled as initrd instead of games suite
DEBUG_INITRD_HELLOWORLD ?= 0

# if you are missing efi firmware for qemu testing you can install via "make efi-install"
AAVMF_CODE 	?= EFI/AAVMF_CODE.fd
AAVMF_VARS 	?= EFI/AAVMF_VARS.fd
OVMF_CODE 	?= EFI/OVMF_CODE.fd
OVMF_VARS 	?= EFI/OVMF_VARS.fd

# interface for external shell scripts to read out variables
.PHONY: print-%
print-%:
	@echo $($*)