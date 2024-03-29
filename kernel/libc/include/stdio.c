#include <vga/vga.h>
#include "stdio.h"


// macros for tracking state in printf function
#define PRINTF_STATE_NORMAL	0
#define PRINTF_STATE_SPEC	1

// convert a numerical value into a string and print to console
/* consider making this return a buffer that can be printed or used anywhere else a string buffer may be used  */
static int* print_number(int* argp, int radix) {
	char buffer[32] = {0};
	unsigned int number = *argp;
	argp += 1;
	int pos = 0;

	// reads the digits to the buffer backwards
	do {
		char rem = number % radix;
		number /= radix;
		buffer[pos++] = (rem < 10) ? (rem + '0') : (rem - 10 + 'a');
	} while(number > 0);

	// prints the buffer to the console backwards, thus making it forwards
	while (--pos >= 0) {
		putc(buffer[pos]);
	}
	return argp;
}

// hacky printf implementation, only supports one basic option
void printf(const char* fmt, ...) {
	int state = PRINTF_STATE_NORMAL;
	int* argp = (int*)&fmt;
	argp++;

	while (*fmt) {
		switch (state) {
			case PRINTF_STATE_NORMAL:
				switch(*fmt) {
					case '%':
						state = PRINTF_STATE_SPEC;
						++fmt;
						goto PRINTF_STATE_SPEC_;
						break;
					default:
						putc(*fmt);
				}
				break;
			case PRINTF_STATE_SPEC:
			PRINTF_STATE_SPEC_:
				switch(*fmt) {
					case '%':
						putc('%');
						break;
					case 'c':
						putc((char)*argp++);
						break;
					case 's':
						puts((char*)*argp++);
						break;
					case 'd':
						argp = print_number(argp, 10);
						break;
					case 'x':
						argp = print_number(argp, 16);
						break;
					default:
						break;
				}
				break;
		}
		state = PRINTF_STATE_NORMAL;
		++fmt;
	}
}
