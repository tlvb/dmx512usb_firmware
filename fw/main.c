#include "usbusart.h"
#include "leds.h"
#ifndef F_CPU
#define F_CPU 20000000
#endif
#include <util/delay.h>

int main(void) {
	led_setup();
	led_hello();
	uu_setup();
	sei();

	uint8_t text[3];
	
	for (;;) {
		led_on(LED0);
		uu_read(text, 3);
		led_off(LED1);
		uu_write(text, 3);
		_delay_ms(125);
	}
}
