TGT ?= i686-elf
AS := $(TGT)-as
CC := $(TGT)-gcc
STRIP := $(TGT)-strip

CFLAGS ?= -O2 -g
LIBS ?=

CFLAGS := $(CFLAGS) -ffreestanding -Wall -Wextra
LIBS := $(LIBS) -nostdlib -lgcc

OBJS = kernel/kmain.o boot/boot.o kernel/drivers/vga.o
LINK_LIST = $(OBJS) $(LIBS)

include kernel/make.config

.PHONY: all clean release
.SUFFIXES: .o .c .S

all: loosipher.elf

release: all
	$(STRIP) loosipher.elf	

loosipher.elf: $(OBJS) linker.ld
	$(CC) -T linker.ld -o $@ $(CFLAGS) $(LINK_LIST)

.c.o:
	$(CC) -MD -c $< -o $@ -std=gnu11 $(CFLAGS) $(KERNEL_INCLUDE_FLAGS)
.S.o:
	$(AS) $< -o $@

clean:
	rm -f loosipher.elf
	find . -name '*.o' -delete
	find . -name '*.d' -delete
