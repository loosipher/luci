#include <stdint.h>

typedef struct {
	uint16_t isr_low;
	uint16_t kernel_cs;
	uint8_t reserved;
	uint8_t attributes;
	uint16_t isr_high;
} __attribute__((packed)) idt_entry_t;

__attribute__((noreturn))
void exception_handler(void);
void exception_handler() {
	__asm__ volatile ("cli; hlt");
}
