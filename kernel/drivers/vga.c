#include "stdint.h"
#include "stddef.h"

enum vga_color {
	VC_BLACK  = 0,
	VC_BLUE   = 1,
	VC_GREEN  = 2,
	VC_CYAN   = 3,
	VC_RED    = 4,
	VC_MGNTA  = 5,
	VC_BROWN  = 6,
	VC_LGRAY  = 7,
	VC_DGRAY  = 8,
	VC_LBLUE  = 9,
	VC_LGREEN = 10,
	VC_LCYAN  = 11,
	VC_LRED   = 12,
	VC_LMGNTA = 13,
	VC_LBROWN = 14,
	VC_WHITE  = 15
};

struct VGAByte {
	uint8_t c;
	uint8_t color;
} __attribute__((packed));

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;
static size_t vga_x = 0;
static size_t vga_y = 0;
static uint8_t color = VC_BLACK<<4 | VC_GREEN;
static struct VGAByte* buffer = (struct VGAByte*) 0x0b8000;

static inline struct VGAByte entry(char c) {
	return (struct VGAByte) { c, color };
}

void clear(void) {
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			buffer[(y*VGA_WIDTH)+x] = entry(' ');
		}
	}
}

void putc(char c) {
	if (c == '\n') {
		vga_x = 0;
		vga_y = (vga_y+1)%VGA_HEIGHT;
	}
	else {
		buffer[(vga_y*VGA_WIDTH)+vga_x] = entry(c);
		vga_x += 1;
		if (vga_x >= VGA_WIDTH) {
			vga_x = 0;
			vga_y = (vga_y+1)%VGA_HEIGHT;
		}
	}
}

void puts(char* s) {
	int b = (int) &s;
	while (*s != 0x00) {
		putc(*(s++));
	}
	s = (char*)b;
}
