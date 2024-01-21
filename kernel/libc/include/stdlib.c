// this uses the linux version of outb, not the osdev wiki's version; keep in mind
void outb(unsigned char value, unsigned short int port) {
	asm volatile ("outb %0, %1"
			:
			:"r"(value), "r"(port)
			:);
}
