#include "board.h"
#include <util/delay.h>

void pinsetup(void) {
	DDRD |= _BV(USBRTS)|_BV(DMXREN)|_BV(DMXTEN)|_BV(DMXTX)|_BV(USBTX);
	PORTD |= _BV(USBRTS)|_BV(USBTX);
	DDRB |= _BV(LED2)|_BV(LED1)|_BV(LED0);
	PORTB |= _BV(PB4);
	LEDS_OFF();
}

void ledhello(void) {
	for (uint8_t i=7; i>4; --i) {
		PORTB &=~ _BV(i);
		_delay_ms(100);
	}
	_delay_ms(200);
	for (uint8_t i=7; i>4; --i) {
		PORTB |= _BV(i);
		_delay_ms(100);
	}
	_delay_ms(200);
}
