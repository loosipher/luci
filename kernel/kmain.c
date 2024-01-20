#include "vga.h"
#include <gdt.h>
#include <idt.h>
#include <isr.h>
#include <stdlib.h>
#include <stdio.h>


void kmain(void) {
	// disable cursor
	outb(0x0a, 0x3d4);
	outb(0x20, 0x3d5);

	clear();
	// print some informational information
	puts("Booted from GRUB.\n");
	printf("Testing printf: 0x%x\n", 0xdeadbeef);
	printf("Loading GDT...\n");

	// load the GDT
	lgdt();
	load_segments();
	printf("GDT loaded.\n");
	printf("Loading IDT...\n");

	// load the IDT
	lidt();
	init_gates();
	for (int i = 0; i < 256; ++i) {
		IDT_enable_gate(i);
	}
	outb(PIC1_DAT, 0xfd);
	outb(PIC2_DAT, 0xff);
	asm volatile ("sti");
	printf("IDT loaded.\n");
}
