#include "../idt.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>


// IDT structures
static struct IdtEntry {
	uint16_t offset_low;
	uint16_t segment;
	uint8_t reserved;
	uint8_t flags;
	uint16_t offset_high;
} __attribute__((packed));

static struct IdtPointer {
	uint16_t limit;
	uint32_t base;
} __attribute__((packed));

// a structure that represents the registers as they are pushed by isr_common in idtS.S
static struct RegisterInfo {
	uint32_t ds;
	uint32_t edi;
	uint32_t esi;
	uint32_t ebp;
	uint32_t esp;
	uint32_t ebx;
	uint32_t edx;
	uint32_t ecx;
	uint32_t eax;
	uint32_t interrupt;
	uint32_t error;
	uint32_t eip;
	uint32_t cs;
	uint32_t eflags;
} __attribute((packed));

__attribute__((noreturn))
static void STOP() {
	asm volatile ("cli; hlt");
}

static void eoi() {

}

// the regular handler called by isr_common
void exception_handler(struct RegisterInfo* regs) {
	switch (regs->interrupt) {
		/*case 0x00:
			printf("Division error. #DE ~ No error code.\n");
			STOP();
			break;

		case 0x01:
			printf("Debug. #DB ~ No error code.\n");
			break;

		case 0x08:
			printf("Double fault encountered! #DF ~ Error code should be zero.\n");
			STOP();
			break;*/

		default:
			printf("Unhandled interrupt: 0x%x\neax: 0x%x ebx: 0x%x ecx:0x%x\nedx: 0x%x\nError code: 0x%x\nebp: 0x%x, cs: 0x%x, eip: 0x%x\n",
					regs->interrupt,
					regs->eax,
					regs->ebx,
					regs->ecx,
					regs->edx,
					regs->error,
					regs->ebp,
					regs->cs,
					regs->eip
				);
			STOP();
			break;
	}
}

static struct IdtEntry IDT[256];
static struct IdtPointer IDT_ptr = (struct IdtPointer) {
	sizeof(IDT)-1,
	(uint32_t)&IDT
};

void IDT_set_gate(int i, void* base, uint16_t segment, uint8_t flags) {
	uint32_t addr = (uint32_t)base;
	IDT[i] = (struct IdtEntry) {
		addr & 0xffff,
		segment,
		0x00,
		flags,
		(addr >> 16) & 0xffff
	};
}

void IDT_enable_gate(int i) {
	IDT[i].flags |= IDT_PRESENT;
}
void IDT_disable_gate(int i) {
	IDT[i].flags &= ~IDT_PRESENT;
}

void IDT_test(int i) {
	uint32_t func_addr = ((uint32_t)IDT[i].offset_low) | ((uint32_t)IDT[i].offset_high << 16);
	void (*func)(void) = (void (*)(void))func_addr;
	func();
}

void PIC_init() {
	outb(PIC1_CMD, 0x11);
	outb(PIC2_CMD, 0x11);
	outb(PIC1_DAT, 0x20);
	outb(PIC2_DAT, 0x28);
	outb(PIC1_DAT, 0x04);
	outb(PIC2_DAT, 0x02);
	outb(PIC1_DAT, 0x01);
	outb(PIC2_DAT, 0x01);
}

// a helper to load the IDT into the IDTR
void lidt(void) {
	asm volatile ("lidt %0"
			:
			: "m"(IDT_ptr)
			:);
}
