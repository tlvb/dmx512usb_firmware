#define F_CPU 20000000
#include <avr/io.h>
#include <util/delay.h>
#include <inttypes.h>

int main(void) {
	uint8_t *text = (uint8_t*) " Hello, World!\n";
	DDRB |= _BV(PB7);
	UCSR0B |= _BV(RXEN0) | _BV(TXEN0);
	UCSR0C |= _BV(UCSZ01) | _BV(UCSZ00);
	UBRR0H = 0; UBRR0L = 0;
	for (;;) {
		for (uint8_t i=32; i<127; ++i) {
			UDR0 = i;
			_delay_ms(1);
			for (uint8_t j=0; j<15; ++j) {
				UDR0 = text[j];
				_delay_ms(1);
			}
			PINB |= _BV(PB7);
			_delay_ms(250);
		}
	}

}
