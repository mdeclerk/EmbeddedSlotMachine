.ONESHELL:
/build/iso/esp:
	if [ ! -f "$(UKI_X86_64)" ] && [ ! -f "$(UKI_AARCH64)" ]; then
		@echo "No kernel image available."; exit 1
	fi
	
	rm -rf $@

	if [ -f "$(UKI_X86_64)" ]; then
		install -m0644 -D $(UKI_X86_64) $@/EFI/BOOT/BOOTX64.EFI
	fi
	if [ -f "$(UKI_AARCH64)" ]; then
		install -m0644 -D $(UKI_AARCH64) $@/EFI/BOOT/BOOTAA64.EFI
	fi

.ONESHELL:
/build/iso/esp.img: /build/iso/esp
	bytes=$$(du -sb $< | awk '{print $$1}')
	bytes=$$((bytes + 1048576))
	size_mib=$$(( (bytes + 1048575) / 1048576 ))
	truncate -s "$${size_mib}M" $@
	mkfs.vfat -n ESP $@
	mmd -i $@ ::/EFI ::/EFI/BOOT
	mcopy -i $@ -s $</* ::/

$(ISO): 
	$(MAKE) /build/iso/esp.img
	xorriso -as mkisofs -r -V "SLOTMACHINE" -o $@ \
		-eltorito-alt-boot -e EFI/esp.img -no-emul-boot -isohybrid-gpt-basdat \
		-graft-points \
		EFI/esp.img=/build/iso/esp.img

.PHONY: isoimage
isoimage: $(ISO)

.PHONY: isoimage-mrproper
isoimage-mrproper:
	rm -rf /build/iso
	rm -f $(ISO)