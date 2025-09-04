/build/kernel/.config:
	@echo "[BUILD] $@"
	@env -i PATH=$(PATH) MAKEFLAGS= MAKEOVERRIDES= sh -lc '\
		cd $(dir $@) && \
		make tinyconfig && \
		scripts/config \
			-e EFI -e EFI_STUB -e ACPI -e PCI -e HOTPLUG_PCI -e SYSFS -e PRINTK -e FUTEX -e BINFMT_ELF -e DEVTMPFS  \
			-e USB_SUPPORT -e USB -e USB_XHCI_HCD -e USB_XHCI_PLATFORM -e INPUT -e INPUT_EVDEV -e HID -e HID_GENERIC -e USB_HID \
			-e TTY \
				-e SERIAL_AMBA_PL011 -e SERIAL_AMBA_PL011_CONSOLE \
				-e DRM -e DRM_SIMPLEDRM -e DRM_FBDEV_EMULATION -e SYSFB_SIMPLEFB -e FRAMEBUFFER_CONSOLE \
			-e BLK_DEV_INITRD --set-str INITRAMFS_SOURCE "/build/initrd.cpio" \
				-d RD_GZIP -d RD_BZIP2 -d RD_LZMA -d RD_XZ -d RD_LZO -d RD_LZ4 -d RD_ZSTD \
			--set-str CMDLINE "rdinit=/bin/init console=ttyAMA0" && \
		make olddefconfig'

/build/kernel/arch/arm64/boot/Image: /build/kernel/.config /build/initrd.cpio
	@echo "[BUILD] $@"
	@env -i PATH=$(PATH) MAKEFLAGS= MAKEOVERRIDES= \
		make --no-print-directory -C /build/kernel -j $(shell nproc) Image

$(UKI_AARCH64): /build/kernel/arch/arm64/boot/Image
	@echo "[COPY] $@"
	@install -D /build/kernel/arch/arm64/boot/Image $@

.PHONY: uki
uki: 
	@if [ ! -f "$(UKI_AARCH64)" ]; then \
		$(MAKE) $(UKI_AARCH64); \
	else \
		echo "[INFO] $(UKI_AARCH64) already exists"; \
	fi

# convenience multi-purpose target to work with kernel while being in /project
.PHONY: uki-%
uki-%:
	@env -i PATH=$(PATH) MAKEFLAGS= MAKEOVERRIDES= \
		make -C /build/kernel -j $(shell nproc) $*