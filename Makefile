# global config variables
include config.mk

# build rules for initramfs user space, unified kernel images, and multi-arch iso image
include rules/initrd.mk
include rules/uki-$(ARCH).mk
include rules/isoimage.mk

# install EFI firmware for qemu testing if needed
include rules/efi-install.mk

# clean build artifacts
.PHONY: clean
clean:
	@echo "[RM]  $(UKI_AARCH64) $(UKI_X86_64) $(ISO)"
	@rm -f $(UKI_AARCH64) $(UKI_X86_64) $(ISO)

# restore fresh build environment
.PHONY: mrproper
mrproper: initrd-mrproper uki-mrproper isoimage-mrproper clean