CC := i686-elf-gcc
AR := i686-elf-ar

OBJS := include/stdlib.o include/stdint.o include/stddef.o include/stdio.o
BINARIES := libk.a
LIBK_OBJS := $(OBJS:.o=.libk.o)

CFLAGS := -g -ffreestanding -Wall -Wextra

.PHONY: all clean
.SUFFIXES: .o .libk.o .c

all: $(BINARIES)

libk.a: $(LIBK_OBJS)
	$(AR) rcs $@ $(LIBK_OBJS)

.c.o:
	$(CC) -MD -c $< -o $@ -std=gnu11 $(CFLAGS)

.c.libk.o:
	$(CC) -MD -c $< -o $@ -std=gnu11 $(CFLAGS)

clean:
	find . -name '*.o' -delete
	find . -name '*.d' -delete
	find . -name '*.a' -delete
