// this uses the linux version of outb, not the osdev wiki's version; keep in mind
void outb(unsigned char value, unsigned short int port) {
	asm volatile ("outb %0, %1"
			:
			:"r"(value), "r"(port)
			:);
}

unsigned char inb(unsigned short int port) {
	unsigned char retval;
	asm volatile ("inb  %1, %0"
			:"=a"(retval)
			:"Nd"(port)
			:"memory");
	return retval;
}
