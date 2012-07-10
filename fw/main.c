#include "usbusart.h"
#ifndef F_CPU
#define F_CPU 20000000
#endif
#include <util/delay.h>

int main(void) {
	uu_setup();
	sei();

	uint8_t text[3];
	
	for (;;) {
		uu_read(text, 3);
		uu_write(text, 3);
		_delay_ms(125);
	}
}
