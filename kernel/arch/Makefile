TGT ?= i686-elf-
CC := $(TGT)gcc
AS := $(TGT)as
AR := $(TGT)ar

HOST ?= x86

GDTOBJS := $(HOST)/gdt.o $(HOST)/gdtS.o
BINARIES := gdt.a
CFLAGS := -g -O2 -ffreestanding -Wall -Wextra
INCLUDE := -Ilibc/include -Idrivers -Iarch -lk


.SUFFIXES: .c .o .S
.PHONY: all clean

all: $(BINARIES)

gdt.a: $(GDTOBJS)
	$(AR) rcs $@ $(GDTOBJS)

.c.o:
	$(CC) -MD -c $< -o $@ -std=gnu11 $(CFLAGS) $(INCLUDE)

.S.o:
	$(AS) $< -o $@

clean:
	find . -name '*.a' -delete
	find . -name '*.d' -delete
	find . -name '*.o' -delete
