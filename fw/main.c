#include "board.h"
#include "usbusart.h"
#include <string.h>
#include <util/delay.h>

extern volatile RINGBUFFER(TXBSZ) txb;

int main(void) {

	pinsetup();

	ledhello();

	uu_init();

	sei();

	uint8_t text[3];

	
	for (;;) {
		LED_ON(LED0);
		uu_read(text, 3);
		LED_OFF(LED0);
		uu_write(text, 3);
		_delay_ms(125);

	}
}

ISR(BAD_ISR_vect) {
	for (;;) {
		ledhello();
	}
}
