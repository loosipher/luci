TGT ?= i686-elf-
AS := $(TGT)as
CC := $(TGT)gcc

CFLAGS ?=
CFLAGS := $(CFLAGS) -g -O2 -ffreestanding -Wall -Wextra
LIBS := -nostdlib -lgcc

OBJS := boot/entry.o
LINK_LIST := $(OBJS) kernel/kmain.o kernel/drivers/vga.o kernel/libc/libk.a kernel/arch/gdt.a kernel/arch/idt.a

.PHONY: all clean rebuild iso
.SUFFIXES: .c .S .o

rebuild: clean all

release: all
	$(TGT)objcopy --only-keep-debug luci luci.sym
	$(TGT)strip --strip-debug --strip-unneeded luci

all: luci

luci: $(OBJS) linker.ld
	$(MAKE) -C kernel all
	$(CC) -T linker.ld -o $@ $(CFLAGS) $(LINK_LIST) $(LIBS)

.S.o:
	$(AS) $< -o $@

.c.o:
	$(CC) -MD -c $< -o $@ $(CFLAGS)

iso: release
	mkdir -p iso/boot/grub
	cp luci iso/boot
	cp grub.cfg iso/boot/grub
	grub-mkrescue -o luci.iso iso

clean:
	rm -rf luci.iso luci.sym luci iso
	find . -name '*.o' -delete
	$(MAKE) -C kernel clean
