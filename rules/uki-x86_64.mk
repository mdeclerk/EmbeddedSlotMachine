/build/kernel/.config:
	@echo "[BUILD] $@"
	@env -i PATH=$(PATH) MAKEFLAGS= MAKEOVERRIDES= sh -lc '\
		cd $(dir $@) && \
		make tinyconfig && \
		scripts/config \
			-e EFI -e EFI_STUB -e 64BIT -e ACPI -e PRINTK -e FUTEX -e BINFMT_ELF -e DEVTMPFS \
			-e TTY \
				-e SERIAL_8250 -e SERIAL_8250_CONSOLE \
				-e DRM -e DRM_SIMPLEDRM -e DRM_FBDEV_EMULATION -e SYSFB_SIMPLEFB -e FRAMEBUFFER_CONSOLE \
				-d VGA_CONSOLE \
			-e BLK_DEV_INITRD --set-str INITRAMFS_SOURCE "/build/initrd.cpio" \
				-d RD_GZIP -d RD_BZIP2 -d RD_LZMA -d RD_XZ -d RD_LZO -d RD_LZ4 -d RD_ZSTD \
			-e CMDLINE_BOOL --set-str CMDLINE "rdinit=/bin/init console=ttyS0" && \
		make olddefconfig'

/build/kernel/arch/x86/boot/bzImage: /build/kernel/.config /build/initrd.cpio
	@echo "[BUILD] $@"
	@env -i PATH=$(PATH) MAKEFLAGS= MAKEOVERRIDES= \
		make --no-print-directory -C /build/kernel -j $(shell nproc) bzImage

$(UKI_X86_64): /build/kernel/arch/x86/boot/bzImage
	@echo "[COPY] $@"
	@install -D /build/kernel/arch/x86/boot/bzImage $@

.PHONY: uki
uki: 
	@if [ ! -f "$(UKI_X86_64)" ]; then \
		$(MAKE) $(UKI_X86_64); \
	else \
		echo "[INFO] $(UKI_X86_64) already exists"; \
	fi

# convenience multi-purpose target to work with kernel while being in /project
.PHONY: uki-%
uki-%:
	@env -i PATH=$(PATH) MAKEFLAGS= MAKEOVERRIDES=  \
		make -C /build/kernel -j $(shell nproc) $*