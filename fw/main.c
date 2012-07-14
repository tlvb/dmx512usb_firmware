#include <avr/io.h>
#include "leds.h"
#include "button.h"

#define DMXDDR DDRD
#define DMXPORT PORTD
#define DMXPIN PIND
#define DMXREN_ PD5
#define DMXTEN PD4
#define DMXTX PD3
#define DMXRX PD2

int main(void) {
	led_setup();
	button_setup();
	DMXDDR |= _BV(DMXREN_)|_BV(DMXTEN)|_BV(DMXTX);
	DMXDDR &= ~_BV(DMXREN_);

	DMXPORT |= _BV(DMXTEN);
	DMXPORT &= ~_BV(DMXREN_);

	for (;;) {
		if (button()) {
			led_on(LED0);
			DMXPORT |= _BV(DMXTX);
		}
		else {
			led_off(LED0);
			DMXPORT &= ~_BV(DMXTX);
		}
		if ((DMXPIN & _BV(DMXRX)) != 0)
			led_on(LED1);
		else
			led_off(LED1);
	}
}
