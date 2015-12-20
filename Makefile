.PHONY: all system headers kernel libc launch-iso launch-kernel clean mrpwoper

KERNEL=sysroot/usr/src/kernel/output/i386/kernel.bin

all: system

launch-iso: iso/img.iso
	qemu-system-i386 -cdrom iso/img.iso

launch-kernel: kernel
	qemu-system-i386 -kernel ${KERNEL}
	
launch-kernel-debug: kernel
	qemu-system-i386 -s -S -kernel ${KERNEL}
	
launch-kernel-bochs: iso/img.iso
	bochs -f bochsrc.txt -q

iso/img.iso: system
	cp ${KERNEL} iso/img/boot/kernel.bin
	grub-mkrescue -o iso/img.iso iso/img
# ^^^^^^^^^^^^^^^ deps: mtools, xorriso, grub-pc-bin

system: headers libc kernel

headers:
	$(MAKE) -C sysroot/usr/src/libc install-headers
	$(MAKE) -C sysroot/usr/src/kernel install-headers

libc: headers
	$(MAKE) -C sysroot/usr/src/libc install-libs

kernel: headers libc
	$(MAKE) -C sysroot/usr/src/kernel

clean:
	$(MAKE) -C sysroot/usr/src/libc clean
	$(MAKE) -C sysroot/usr/src/kernel clean

mrpwoper: clean
	$(MAKE) -C sysroot/usr/src/libc mrpwoper
	$(MAKE) -C sysroot/usr/src/kernel mrpwoper
	rm -f iso/img.iso
