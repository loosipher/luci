#include <stdint.h>

// macros useful for interacting with the PIC
#define PIC1_CMD 0x20
#define PIC1_DAT 0x21
#define PIC2_CMD 0xa0
#define PIC2_DAT 0xa1

// constants for IDT flags
typedef enum {
	IDT_GATE_TASK		= 0x05,
	IDT_GATE_16B_INT	= 0x06,
	IDT_GATE_16B_TRAP	= 0x07,
	IDT_GATE_32B_INT	= 0x0e,
	IDT_GATE_32B_TRAPS	= 0x0f,
	IDT_RING_0		= 0x00 << 5,
	IDT_RING_1		= 0x01 << 5,
	IDT_RING_2		= 0x02 << 5,
	IDT_RING_3		= 0x03 << 5,
	IDT_PRESENT		= 0x80
};
__attribute__((noreturn)) void exceptions_handler(void);
void IDT_set_gate(int i, void* base, uint16_t segment, uint8_t flags);
void IDT_enable_gate(int i);
void IDT_disable_gate(int i);
void IDT_test(int i);
void PIC_init(void);
void lidt(void);
