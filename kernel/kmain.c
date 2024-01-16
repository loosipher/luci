#include "vga.h"
#include <gdt.h>
#include <stdlib.h>


void kmain(void) {
	// disable cursor
	outb(0x0a, 0x3d4);
	outb(0x20, 0x3d5);

	clear();
	// print some informational information
	puts("Booted from GRUB.\n\0");
	puts("Loading GDT...\n\0");

	//
	// load the GDT
	//
	lgdt();
	load_segments();
	puts("GDT loaded.\n\0");
}
