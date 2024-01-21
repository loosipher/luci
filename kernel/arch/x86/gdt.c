#include <stdint.h>

// 
// GDT structures
//
// A single entry in the GDT
struct GdtEntry {
	uint16_t limit_low;
	uint16_t base_low;
	uint8_t base_mid;
	uint8_t access;
	uint8_t limhi_flg;
	uint8_t base_high;
} __attribute__((packed));

// pointer to GDT
struct GdtPointer {
	uint16_t size;
	uint32_t offset;
} __attribute__((packed));

// the GDT itself and its pointer
static struct GdtEntry volatile GDT[] = {
	(struct GdtEntry){0},
	(struct GdtEntry){
		0xffff,
		0x0000,
		0x00,
		0x9a,		// kernel code segment
		0xcf,		// page granularity | 32-bit mode selector
		0x00
	},
	(struct GdtEntry){
		0xffff,
		0x0000,
		0x00,
		0x92,		// kernel data segment
		0xcf,
		0x00
	},
	(struct GdtEntry){
		0xffff,
		0x0000,
		0x00,
		0xfa,		// user code segment
		0xcf,
		0x00
	},
	(struct GdtEntry){
		0xffff,
		0x0000,
		0x00,
		0xf2,		// user data segment
		0xcf,
		0x00
	}
};
struct GdtPointer ptr = (struct GdtPointer){
	sizeof(GDT)-1,
	(uint32_t)&GDT
};

// helper function to load the GDT
void lgdt(void) {
	asm volatile (
			"lgdt %0\n"
			:
			: "m" (ptr)
			:);
}
