#define F_CPU 20000000
#include <avr/io.h>
#include <util/delay.h>
#include <inttypes.h>

int main(void) {
	DDRB = 0xe0;
	uint8_t i = 0;
	for (;;) {
		PORTB &= 0x1f;
		PORTB |= ((i++)&7) << 5;
		_delay_ms(250);
	}
}

