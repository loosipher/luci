TGT ?= i686-elf
CC := $(TGT)-gcc

CFLAGS := -g -O2 -ffreestanding -Wall -Wextra
OBJS := 
INCLUDE := -Ilibc/include -Idrivers -Iarch


.SUFFIXES: .c .o
.PHONY: all clean

all: $(OBJS)
	$(MAKE) -C drivers all
	$(MAKE) -C libc all
	$(MAKE) -C arch all
	$(CC) -MD -c kmain.c -o kmain.o -std=gnu11 $(CFLAGS) $(INCLUDE)

.c.o:
	$(CC) -MD -c $< -o $@ -std=gnu11 $(CFLAGS) $(INCLUDE)

clean:
	$(MAKE) -C drivers clean
	$(MAKE) -C libc clean
	$(MAKE) -C arch clean
	find . -name '*.d' -delete
	find . -name '*.o' -delete
