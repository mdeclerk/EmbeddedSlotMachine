/build/iso/esp:
	@echo "[BUILD] $@"
	@if [ ! -f "$(UKI_X86_64)" ] && [ ! -f "$(UKI_AARCH64)" ]; then \
		echo "No kernel image available."; exit 1; \
	fi
	
	@rm -rf $@

	@if [ -f "$(UKI_X86_64)" ]; then \
		install -D $(UKI_X86_64) $@/EFI/BOOT/BOOTX64.EFI; \
	fi
	@if [ -f "$(UKI_AARCH64)" ]; then \
		install -D $(UKI_AARCH64) $@/EFI/BOOT/BOOTAA64.EFI; \
	fi

/build/iso/esp.img: /build/iso/esp
	@echo "[BUILD] $@" && \
	bytes=$$(du -sb $< | awk '{print $$1}') && \
	bytes=$$((bytes + 1048576)) && \
	size_mib=$$(( (bytes + 1048575) / 1048576 )) && \
	truncate -s "$${size_mib}M" $@ && \
	mkfs.vfat -n ESP $@ && \
	mmd -i $@ ::/EFI ::/EFI/BOOT && \
	mcopy -i $@ -s $</* ::/

$(ISO): /build/iso/esp.img
	@echo "[BUILD] $@"
	@xorriso -as mkisofs -r -V "SLOTMACHINE" -o $@ \
		-eltorito-alt-boot -e EFI/esp.img -no-emul-boot -isohybrid-gpt-basdat \
		-graft-points \
		EFI/esp.img=/build/iso/esp.img

.PHONY: isoimage
isoimage: 
	@if [ ! -f "$(ISO)" ]; then \
		$(MAKE) $(ISO); \
	else \
		echo "[INFO] $(ISO) already exists"; \
	fi

.PHONY: isoimage-mrproper
isoimage-mrproper:
	@rm -rf /build/iso
	@rm -f $(ISO)