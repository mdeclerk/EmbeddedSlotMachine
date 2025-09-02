$(OVMF_CODE): /usr/share/OVMF/OVMF_CODE_4M.fd
	install -D $< $@

$(OVMF_VARS): /usr/share/OVMF/OVMF_VARS_4M.fd
	install -D $< $@

$(AAVMF_CODE): /usr/share/AAVMF/AAVMF_CODE.no-secboot.fd
	install -D $< $@

$(AAVMF_VARS): /usr/share/AAVMF/AAVMF_VARS.fd
	install -D $< $@

.PHONY: efi-reinstall efi-clean efi-reinstall

efi-install: $(OVMF_CODE) $(OVMF_VARS) $(AAVMF_CODE) $(AAVMF_VARS)

efi-clean:
	rm -f $(OVMF_CODE) $(OVMF_VARS) $(AAVMF_CODE) $(AAVMF_VARS)	

efi-reinstall: efi-clean efi-install