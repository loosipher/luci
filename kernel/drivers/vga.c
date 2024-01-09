#include "vga.h"
#include <stdint.h>
#include <stddef.h>

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
	uint8_t color;
	uint8_t c;
} __attribute__((packed));

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;
static size_t vga_x = 0;
static size_t vga_y = 0;
static uint8_t color = VC_BLACK<<4 | VC_GREEN;
static struct VGAByte* buffer = (struct VGAByte*) 0x0b8000;

static inline struct VGAByte entry(char c) {
	return (struct VGAByte) { color, c };
}

void clear(void) {
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			buffer[(y*VGA_WIDTH)+x] = entry('.');
		}
	}
}
