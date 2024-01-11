TGT ?= i686-elf-
AS := $(TGT)as
CC := $(TGT)gcc
STRIP := $(TGT)strip

CFLAGS ?=
CFLAGS := $(CFLAGS) -g -O2 -ffreestanding -Wall -Wextra
LIBS := -nostdlib -lgcc

OBJS := boot/entry.o kernel/kmain.o
LINK_LIST := $(OBJS) kernel/drivers/vga.o

.PHONY: all clean rebuild
.SUFFIXES: .c .S .o

rebuild: clean all

all: luci

luci: $(OBJS) linker.ld
	$(MAKE) -C kernel all
	$(CC) -T linker.ld -o $@ $(CFLAGS) $(LINK_LIST) $(LIBS)
	$(AS) boot/boot.S -o boot/boot.o
	$(TGT)objcopy -O binary boot/boot.o boot.bin
	dd if=/dev/zero of=loosipher.img bs=512 count=10000
	mkfs.vfat -F12 loosipher.img
	dd if=boot.bin of=loosipher.img skip=62 seek=62 bs=1 count=448 conv=notrunc
	mcopy -i loosipher.img $@ ::/

.S.o:
	$(AS) $< -o $@

.c.o:
	$(CC) -MD -c $< -o $@ $(CFLAGS)

clean:
	rm -f loosipher.img luci boot.bin
	find . -name '*.o' -delete
	$(MAKE) -C kernel clean
