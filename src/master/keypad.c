#include "keypad.h"
#include "avr.h"

unsigned char is_pressed(unsigned char r, unsigned char c) {
	//     PC: 7 6 5 4 3 2 1 0
	// keypad: 1 2 3 4 5 6 7 8
	// keypad: c4 c3 c2 c1 r4 r3 r2 r1
	
	//DDRC = 0x00;
	//PORTC = 0x00;
	DDRC = 0x01 << r;
	PORTC = 0x10 << c;

	avr_wait(1); //wait a little time to let the port change to the state we want, otherwise the Port/Pin state may still be in the last state and it can lead to some error keys
	if (GET_BIT(PINC, c+4) == 0) {
		return 1;
	}
	return 0;
}

unsigned char is_pressed2(unsigned char k) {
	//     PC: 7 6 5 4 3 2 1 0
	// keypad: 1 2 3 4 5 6 7 8
	// keypad: c4 c3 c2 c1 r4 r3 r2 r1
	
	//DDRC = 0x00;
	//PORTC = 0x00;

	unsigned char r = (k-1)/4;
	unsigned char c = (k-1)%4;
	DDRC = 0x01 << r;
	PORTC = 0x10 << c;

	avr_wait(1); //wait a little time to let the port change to the state we want, otherwise the Port/Pin state may still be in the last state and it can lead to some error keys
	if (GET_BIT(PINC, c+4) == 0) {
		return 1;
	}
	return 0;
}

unsigned char get_key() {
	unsigned char i, j;
	unsigned k = 0;
	for (i=0; i < 4; i++) {
		for (j=0; j < 4; j++) {
			if (is_pressed(i, j) && !k) {
				k = 4 * i + j + 1;
				//return 4 * i + j + 1; If we return here, the time of each get_key() call will be different. We want a stable time.
			}
		}
	}
	return k;
}