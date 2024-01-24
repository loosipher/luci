CC := i686-elf-gcc
AR := i686-elf-ar

OBJS := vga/vga.o kbd/kbd.o
BINARIES := drivers.a
INCLUDE := -I../libc/include
HEADERS := $(OBJS:.o=.h)

CFLAGS := -g -ffreestanding -Wall -Wextra

.PHONY: all clean
.SUFFIXES: .o .c .h

all: $(BINARIES)
	$(foreach hfile,$(HEADERS), \
		$(shell cp $(basename $(hfile).h) .))

drivers.a: $(OBJS)
	$(AR) rcs $@ $(OBJS)

.c.o:
	$(CC) -MD -c $< -o $@ -std=gnu11 $(CFLAGS) $(INCLUDE)

clean:
	rm -f *.h
	find . -name '*.o' -delete
	find . -name '*.d' -delete
	find . -name '*.a' -delete
