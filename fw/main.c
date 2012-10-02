#include "dmxtx.h"
#include "leds.h"
#include "usbusart.h"
#include <avr/io.h>

extern volatile uint8_t dt_txb[DT_TXBSZ];

int main(void) {
	led_setup();
	led_hello();
	uu_setup();

	uint8_t init = 1;
	uint8_t cv[2];

	for (uint8_t i=0; i<DT_TXBSZ; ++i) {
		dt_txb[i] = i;
	}

	dmxtx_setup();
	sei();

	for (;;) {
		uu_read(cv, 2);

		if (init != 0 || cv[0] >= DT_TXBSZ) {
			led_error();
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
			led_off(LED0);
			led_on(LED1);
			dt_txb[cv[0]] = cv[1];
			uu_write(cv, 2);
		}
	}
}
