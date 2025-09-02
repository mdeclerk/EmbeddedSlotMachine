ifeq ($(INITRD_DEBUG),1)

.ONESHELL:
/build/initrd:
	rm -rf $@ && mkdir -p $@/dev
	mknod -m 600 $@/dev/console c 5 1
	mknod -m 666 $@/dev/null    c 1 3
	printf "%s\n%s" \
		 '#include <iostream>' \
		 'int main() { std::cout << "Hello World\n"; for (;;); }' \
		 > /build/helloworld.cpp
	mkdir -p $@/bin
	g++ -static /build/helloworld.cpp -o $@/bin/init
	rm /build/helloworld.cpp

else

.ONESHELL:
/build/slotmachine:
	mkdir -p $@/obj $@/bin
	$(MAKE) -C /project/SlotMachine OBJ_DIR=$@/obj BIN_DIR=$@/bin
	strip $@/bin/*

.ONESHELL:
/build/initrd: /build/slotmachine
	rm -rf $@ && mkdir $@

	mkdir -p $@/dev
	mknod -m 600 $@/dev/console c 5 1
	mknod -m 666 $@/dev/null    c 1 3

	mkdir -p $@/etc/terminfo/l $@/etc/terminfo/v
	cp /usr/share/terminfo/l/linux $@/etc/terminfo/l/linux
	cp /usr/share/terminfo/v/vt100 $@/etc/terminfo/v/vt100

	if [ -f rules/install_to_initramfs.sh ]; then
		rules/install_to_initramfs.sh $</bin $@
	else
		echo "rules/install_to_initramfs.sh not found"; exit 1
	fi

endif

/build/initrd.cpio: /build/initrd
	cd $< && find . | cpio -H newc -o > $@

.PHONY: initrd
initrd: /build/initrd.cpio

.PHONY: initrd-mrproper
initrd-mrproper:
	rm -rf /build/initrd /build/initrd.cpio /build/slotmachine