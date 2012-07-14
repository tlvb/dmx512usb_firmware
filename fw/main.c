#include "dmxtx.h"
#include "leds.h"
/*
#include "button.h"
#include "usbusart.h"
*/
#include <avr/io.h>

extern volatile uint8_t dt_state;
extern volatile uint8_t dt_txb[DT_TXBSZ];
/*
#define h2x(h) ((((h)>>4)>9)?('a'+((h)>>4)-10):('0'+((h)>>4)))
#define l2x(l) ((((l)&7)>9)?('a'+((l)&7)-10):('0'+((l)&7)))
*/
int main(void) {
	/* button_setup(); */
	led_setup();
	led_hello();

	for (uint8_t i=0; i<DT_TXBSZ; ++i) {
		dt_txb[i] = i;
	}

	dmxtx_setup();
	sei();

	for (;;);
}
