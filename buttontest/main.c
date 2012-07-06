#define F_CPU 20000000
#include <avr/io.h>
#include <util/delay.h>
#include <inttypes.h>

int main(void) {
	DDRB = 0xe0;
	PORTB |= 0x10;
	for (;;) {
		if (PINB & _BV(PB4)) {
			PORTB |= 0xe0;
		}
		else {
			PORTB &= 0x1f;
		}
	}
}

