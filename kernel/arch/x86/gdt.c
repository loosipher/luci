#include <stdint.h>

struct GdtEntry {
	uint16_t limit_low;
	uint16_t base_low;
	uint8_t base_mid;
	uint8_t access;
	uint8_t limhi_flg;
	uint8_t base_high;
} __attribute__((packed));

struct GdtPointer {
	uint16_t size;
	uint32_t offset;
} __attribute__((packed));

static struct GdtEntry volatile GDT[5];
void lgdt(void) {
	GDT[0] = (struct GdtEntry){0};
	GDT[1] = (struct GdtEntry){
		0xffff,
		0x0000,
		0x00,
		0x9a,
		0xcf,
		0x00
	};
	GDT[2] = (struct GdtEntry){
		0xffff,
		0x0000,
		0x00,
		0x92,	
		0xcf,
		0x00
	};
	GDT[3] = (struct GdtEntry){
		0xffff,
		0x0000,
		0x00,
		0xfa,
		0xcf,
		0x00
	};
	GDT[4] = (struct GdtEntry){
		0xffff,
		0x0000,
		0x00,
		0xf2,
		0xcf,
		0x00
	};

	struct GdtPointer ptr = (struct GdtPointer){
		sizeof(GDT)-1,
		(uint32_t)&GDT
	};
	uint32_t GDTR = (uint32_t)&ptr;
	asm volatile (
			"lgdt %0\n"
			:
			: "m" (ptr)
			:);
}
