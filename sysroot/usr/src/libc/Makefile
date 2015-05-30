include config.mk

CFLAGS?=-O2 -g
CPPFLAGS?=
LDFLAGS?=
LIBS?=

DESTDIR?=
PREFIX?=usr/
EXEC_PREFIX?=$(PREFIX)
INCLUDEDIR?=$(PREFIX)include
LIBDIR?=$(EXEC_PREFIX)lib

CFLAGS:=$(CFLAGS) -Wall -Wextra
CPPFLAGS:=$(CPPFLAGS) -ffreestanding -D__is_myos_libc -Iinclude -I${SYSROOT}${INCLUDEDIR}  -I${SYSROOT}../toolchain/lib/gcc/i386-unknown-elf/4.9.1/include
LIBK_CFLAGS:=$(CFLAGS) -ffreestanding -fbuiltin -Iinclude -I${SYSROOT}/${INCLUDEDIR} -I${SYSROOT}../toolchain/lib/gcc/i386-unknown-elf/4.9.1/include -g
LIBK_CPPFLAGS:=$(CPPFLAGS) -D__is_myos_kernel

ARCHDIR:=arch/$(SYSTEM_ARCH)

include $(ARCHDIR)/make.config

CFLAGS:=$(CFLAGS) $(ARCH_CFLAGS)
CPFLAGS:=$(CPPFLAGS) $(ARCH_CPPFLAGS)
LIBK_CFLAGS:=$(LIBK_CFLAGS) $(KERNEL_ARCH_CFLAGS)
LIBK_CPFLAGS:=$(LIBK_CPPFLAGS) $(KERNEL_ARCH_CPPFLAGS)
FREEOBJS:=\
$(ARCH_FREEOBJS) \
stdio/printf.o \
stdio/putchar.o \
stdio/puts.o \
stdlib/abort.o \
string/memcmp.o \
string/memcpy.o \
string/memmove.o \
string/memset.o \
string/strlen.o \

HOSTEDOBJS:=\
$(ARCH_HOSTEDOBJS) \

OBJS:=\
$(FREEOBJS) \
$(HOSTEDOBJS) \

LIBK_OBJS:=$(FREEOBJS:.o=.libk.o)

BINARIES=output/${SYSTEM_ARCH}/libc.a output/${SYSTEM_ARCH}/libg.a output/${SYSTEM_ARCH}/libk.a

all: $(BINARIES)

.PHONY: all clean install install-headers install-libs

output/${SYSTEM_ARCH}/libc.a: $(OBJS)
	$(AR) rcs $@ $(OBJS)

output/${SYSTEM_ARCH}/libg.a:
	$(AR) rcs $@

output/${SYSTEM_ARCH}/libk.a: $(LIBK_OBJS)
	$(AR) rcs $@ $(LIBK_OBJS)

%.o: %.c
	$(CC) -c $< -o $@ -std=gnu11 $(CFLAGS) $(CPPFLAGS)

%.o: %.S
	$(CC) -c $< -o $@ $(CFLAGS) $(CPPFLAGS)

%.libk.o: %.c
	$(CC) -c $< -o $@ -std=gnu11 $(LIBK_CFLAGS) $(LIBK_CPPFLAGS)

%.libk.o: %.S
	$(CC) -c $< -o $@ $(LIBK_CFLAGS) $(LIBK_CPPFLAGS)

clean:
	rm -f $(OBJS) $(LIBK_OBJS) *.o */*.o */*/*.o
mrpwoper: clean
	rm -f $(BINARIES)

install: install-headers install-libs

install-headers:
	mkdir -p $(SYSROOT)$(INCLUDEDIR)
	cp -RTv include $(SYSROOT)$(INCLUDEDIR)

install-libs: $(BINARIES)
	mkdir -p $(SYSROOT)$(LIBDIR)
	cp $(BINARIES) $(SYSROOT)$(LIBDIR)
