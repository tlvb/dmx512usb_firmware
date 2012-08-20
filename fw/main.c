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


	uint8_t init = 1;
	uint8_t cv[2];

	led_hello();

	for (;;) {
		uu_read(cv, 2);

		if (init != 0 || cv[0] >= 32) {
			for (uint8_t i=0; i<100; ++i) {
				uu_read(cv, 1);
				if (cv[0] != 0xaa)
					i = 0;
			}
			while (cv[0] != 0x55)
				uu_read(cv, 1);
			init = 0;
		}
		else {
			leds_off();
			if (cv[1]>63)
				led_on(LED0);
			if (cv[1]>127)
				led_on(LED1);
			if (cv[1]>191)
				led_on(LED2);
		}
	}
}
