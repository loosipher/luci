void outb(unsigned char value, unsigned short int port) {
	asm volatile ("outb %0, %1"
			:
			:"r"(value), "r"(port)
			:);
}
