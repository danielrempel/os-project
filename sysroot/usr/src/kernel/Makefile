include config.mk

KERNEL=output/${KERNEL_ARCH}/kernel.bin

CFLAGS?=-O2 -g
PREFIX?=/usr
EXEC_PREFIX?=$(PREFIX)
BOOTDIR?=$(EXEC_PREFIX)/boot
INCLUDEDIR?=$(PREFIX)/include
LIBDIR?=$(EXEC_PREFIX)/lib

CFLAGS:=$(CFLAGS) -ffreestanding -Wall -Wextra
CPPFLAGS:=$(CPPFLAGS) -D__is_myos_kernel -Iinclude -I${SYSROOT}/${INCLUDEDIR}  -I${SYSROOT}../toolchain/lib/gcc/i386-unknown-elf/4.9.1/include
LDFLAGS:=$(LDFLAGS) -L${SYSROOT}/${LIBDIR}
LIBS:=$(LIBS) -nostdlib -lk -lgcc

ARCHDIR:=arch/$(KERNEL_ARCH)

include $(ARCHDIR)/make.config

CFLAGS:=$(CFLAGS) $(KERNEL_ARCH_CFLAGS)
CPFLAGS:=$(CPPFLAGS) $(KERNEL_ARCH_CPPFLAGS)
LDFLAGS:=$(LDFLAGS) $(KERNEL_ARCH_LDFLAGS)
LIBS:=$(LIBS) $(KERNEL_ARCH_LIBS)

OBJS:=\
$(KERNEL_ARCH_OBJS) \
kernel/kernel.o \

CRTI_OBJ:=$(ARCHDIR)/crti.o
CRTBEGIN_OBJ:=$(shell $(CC) $(CFLAGS) $(LDFLAGS) -print-file-name=crtbegin.o)
CRTEND_OBJ:=$(shell $(CC) $(CFLAGS) $(LDFLAGS) -print-file-name=crtend.o)
CRTN_OBJ:=$(ARCHDIR)/crtn.o

ALL_OUR_OBJS:=\
$(CRTI_OBJ) \
$(OBJS) \
$(CRTN_OBJ) \

OBJ_LINK_LIST:=\
$(CRTI_OBJ) \
$(CRTBEGIN_OBJ) \
$(OBJS) \
$(CRTEND_OBJ) \
$(CRTN_OBJ) \

.PHONY: all install-headers clean mrpwoper

all: ${KERNEL}
${KERNEL}: $(OBJ_LINK_LIST) $(ARCHDIR)/linker.ld
	$(CC) -T $(ARCHDIR)/linker.ld -o $@ $(CFLAGS) $(OBJ_LINK_LIST) $(LDFLAGS) $(LIBS)
	
%.o: %.c
	$(CC) -c $< -o $@ -std=gnu11 $(CFLAGS) $(CPPFLAGS)

%.o: %.s
	$(CC) -c $< -o $@ $(CFLAGS) $(CPPFLAGS)

install-headers:
	mkdir -p $(SYSROOT)$(INCLUDEDIR)
	cp -RTv include $(SYSROOT)$(INCLUDEDIR)

clean:
	-rm -f $(OBJS) $(ALL_OUR_OBJS) *.o */*.o */*/*.o

mrpwoper: clean
	-rm ${KERNEL}
