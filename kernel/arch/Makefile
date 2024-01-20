TGT ?= i686-elf-
CC := $(TGT)gcc
AS := $(TGT)as
AR := $(TGT)ar

HOST ?= x86

GDTOBJS := $(HOST)/gdt.o $(HOST)/gdtS.o
IDTOBJS := $(HOST)/idt.o $(HOST)/idtS.o $(HOST)/isr.o
BINARIES := gdt.a idt.a
CFLAGS := -g -O2 -ffreestanding -Wall -Wextra
INCLUDE := -I../libc/include -I../drivers -lk


.SUFFIXES: .c .o .S
.PHONY: all clean stubs

all: stubs $(BINARIES)

stubs:
	cd $(HOST) && perl stub_header_generator.pl

gdt.a: $(GDTOBJS)
	$(AR) rcs $@ $(GDTOBJS)

idt.a: $(IDTOBJS)
	$(AR) rcs $@ $(IDTOBJS)

.c.o:
	$(CC) -MD -c $< -o $@ -std=gnu11 $(CFLAGS) $(INCLUDE)

.S.o:
	$(AS) $< -o $@

clean:
	rm -f $(HOST)/isr.c
	find . -name '*.a' -delete
	find . -name '*.d' -delete
	find . -name '*.o' -delete
