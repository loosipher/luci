#include "kbd.h"
#include <stdbool.h>
#include <stdio.h>


// a queue for sending commands to the keyboard
static char cmd_queue[256] = {0};
static short dq_idx = 0;

// keep track of the currently read code, the exp_flag marks if the expansion byte is sent
static unsigned char current_code = 0;
static bool exp_flag = false;

// keep track of whether shift is pressed
static bool old_shift = false;
static bool shift = false;


// these are the keys in order of their appearance in scan set 1
// capital letters are special keys
static unsigned char characters[] = "A1234567890-=BCqwertyuiop[]DEasdfghjkl;'`F\\zxcvbnm,./G*H IJKLMNOPQRSTU789-456+1230.   VW";
static unsigned char shift_char[] = " !@#$%^&*()_+  QWERTYUIOP{}  ASDFGHJKL:\"~ |ZXCVBNM<>?                                   ";

// helper function to find if the key pressed corresponds to a capital
static bool searchstr(char sub, char* string) {
	char* beg = string;
	while (*string) {
		if (*string == sub) {
			string = beg;
			return true;
		}
		++string;
	}
	string = beg;
	return false;
}

struct Keypress process_code(unsigned char code) {
	struct Keypress kp = {0};
	current_code = code;
	if (current_code == 0xe0) {
		exp_flag = true;
		return kp;
	}
	if (!exp_flag) {
		short key_code = 0;
		char ascii = 0;
		char pressed = 0;
		if (current_code >= 0x01 && current_code <= 0x58) {
			key_code = current_code - 0x01;
			ascii = characters[key_code];
			pressed = true;
		}
		else if (current_code >= 0x81 && current_code <= 0xd8) {
			key_code = current_code - 0x81;
			ascii = characters[key_code];
			pressed = false;
		}
		if (searchstr(ascii, "ABCDEFGHIJKLMNOPQRSTUVW")) {
			switch (ascii) {
				case 'A':
					ascii = '\x1b';
					break;
				case 'B':
					ascii = '\x08';
					break;
				case 'C':
					ascii = '\x09';
					break;
				case 'D':
					ascii = '\x0a';
					break;
				case 'F':
					ascii = '\x00';
					if (old_shift && !pressed) shift = false;
					else if (!old_shift && pressed) shift = true;
					break;
				case 'G':
					ascii = '\x00';
					if (old_shift && !pressed) shift = false;
					else if (!old_shift && pressed) shift = true;
					break;
				default:
					ascii = '\x00';
			}
		}
		if (shift) {
			char new_ascii = shift_char[key_code];
			if (new_ascii != ' ') ascii = new_ascii;
		}
		kp = (struct Keypress) { (char)key_code, ascii, pressed, shift };
		old_shift = shift;
	}
	return kp;
}
