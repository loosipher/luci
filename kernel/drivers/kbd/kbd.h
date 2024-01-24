#include <stdbool.h>

#define KBD_DAT 0x60
#define KBD_CMD 0x64


// packet to send wherever it needs to go
struct Keypress {
	unsigned char key_code;
	unsigned char ascii;
	bool pressed;
	bool shift_pressed;
};

struct Keypress process_code(unsigned char code);
