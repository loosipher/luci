#include "../idt.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>


//
// IDT structures
// 
// A single entry in a table
static struct IdtEntry {
	uint16_t offset_low;
	uint16_t segment;
	uint8_t reserved;
	uint8_t flags;
	uint16_t offset_high;
} __attribute__((packed));

// The pointer to the IDT
static struct IdtPointer {
	uint16_t limit;
	uint32_t base;
} __attribute__((packed));

// the actual IDT and pointer
static struct IdtEntry IDT[256];
static struct IdtPointer IDT_ptr = (struct IdtPointer) {
	sizeof(IDT)-1,
	(uint32_t)&IDT
};

// add the entry to the IDT
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

// set the present bit
void IDT_enable_gate(int i) {
	IDT[i].flags |= IDT_PRESENT;
}
// clear the present bit
void IDT_disable_gate(int i) {
	IDT[i].flags &= ~IDT_PRESENT;
}

// test call an interrupt
void IDT_test(int i) {
	uint32_t func_addr = ((uint32_t)IDT[i].offset_low) | ((uint32_t)IDT[i].offset_high << 16);
	void (*func)(void) = (void (*)(void))func_addr;
	func();
}

//
// Interrupt/exception handler
//
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
} __attribute__((packed));

// stop execution in case of unrecoverable error
__attribute__((noreturn))
static void STOP() {
	asm volatile ("cli; hlt");
}

static inline void eoi() { /* send the eoi command to let the pic know we're done processing the interrupt  */ }

// just used for annotation
static inline void deprecated() {}
static inline void reserved() {}

// the regular handler called by isr_common
void handler(struct RegisterInfo* regs) {
	switch (regs->interrupt) {
		case 0x00:
			printf("Division error. #DE ~ No error code.\n");
			STOP();
			break;

		case 0x01:
			printf("Debug. #DB ~ No error code.\n");
			break;

		case 0x02:
			printf("Non-maskable interrupt. --- ~ No error code.\n");
			STOP();
			break;

		case 0x03:
			printf("Breakpoint. #BP ~ No error code.\n");
			STOP();
			break;

		case 0x04:
			printf("Overflow. #OF ~ No error code.\n");
			STOP();
			break;

		case 0x05:
			printf("Bound range exceeded. #BR ~ No error code.\n");
			STOP();
			break;

		case 0x06:
			printf("Invalid opcode. #UD ~ No error code.\n");
			printf("eip: %x\n", regs->eip);
			STOP();
			break;

		case 0x07:
			printf("Device not available. #NM ~ No error code.\n");
			STOP();
			break;

		case 0x08:
			printf("Double fault encountered! #DF ~ Error code should be zero.\nError: %x\n", regs->error);
			STOP();
			break;

		case 0x09:
			deprecated();
			break;

		case 0x0a:
			printf("Invalid TSS. #TS\nError: %x\n", regs->error);
			STOP();
			break;

		case 0x0b:
			printf("Segment not present. #NP\nError: %x\n", regs->error);
			STOP();
			break;

		case 0x0c:
			printf("Stack segment fault. #SS\nError: %x\n", regs->error);
			STOP();
			break;

		case 0x0d:
			printf("General protection fault. #GP\nError %x\n", regs->error);
			STOP();
			break;

		case 0x0e:
			printf("Page fault. #PF\nError: %x\n", regs->error);
			STOP();
			break;

		case 0x0f:
			reserved();
			break;

		case 0x10:
			printf("x87 floating-point exception. #MF ~ No error code.\n");
			STOP();
			break;

		case 0x11:
			printf("Alignment check. #AC\nError: %x\n", regs->error);
			STOP();
			break;

		case 0x12:
			printf("Machine check. #MC ~ No error code.\n");
			STOP();
			break;

		case 0x13:
			printf("SIMD floating-point exception. #XM/#XF ~ No error code.\n");
			STOP();
			break;

		case 0x14:
			printf("Virtualization exception. #VE ~ No error code.\n");
			STOP();
			break;

		case 0x15:
			printf("Control protection exception. #CP\nError: %x\n", regs->error);
			STOP();
			break;

		case 0x16:
		case 0x17:
		case 0x18:
		case 0x19:
		case 0x1a:
		case 0x1b:
			reserved();
			break;

		case 0x1c:
			printf("Hypervisor injection exception. #HV ~ No error code.\n");
			STOP();
			break;

		case 0x1d:
			printf("VMM communication exception. #VC\nError: %x\n", regs->error);
			STOP();
			break;

		case 0x1e:
			printf("Security exception. #SX\nError: %x\n", regs->error);
			STOP();
			break;

		case 0x1f:
			reserved();
			break;

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

// helper function to remap PIC
static inline void wait(void) {
	outb(0x80, 0);
}

// do the remapping of the PIC
void PIC_init(void) {
	outb(0x11, PIC1_CMD);	// initialization word
	wait();
	outb(0x11, PIC2_CMD);
	wait();
	outb(0x20, PIC1_DAT);	// remap to 0x20
	wait();
	outb(0x28, PIC2_DAT);	// remap to 0x28
	wait();
	outb(0x04, PIC1_DAT);	// tell master slave irq
	wait();
	outb(0x02, PIC2_DAT);	// tell slave its identity
	wait();
	outb(0x01, PIC1_DAT);
	wait();
	outb(0x01, PIC2_DAT);
	wait();
}

// a helper to load the IDT into the IDTR
void lidt(void) {
	asm volatile ("lidt %0"
			:
			: "m"(IDT_ptr)
			:);
}
