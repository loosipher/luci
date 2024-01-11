TGT ?= i686-elf
CC := $(TGT)-gcc

CFLAGS := -g -O2 -ffreestanding -Wall -Wextra
OBJS := drivers/vga.o


.SUFFIXES: .c .o
.PHONY: all clean

all: $(OBJS)

.c.o:
	$(CC) -MD -c $< -o $@ -std=gnu11 $(CFLAGS)

clean:
	find . -name '*.d' -delete
	find . -name '*.o' -delete
