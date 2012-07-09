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

	uint8_t *text = (uint8_t*) "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

for (uint16_t i=0; i<0x1000; ++i) {

//	UDR0 = 'X'; //d
//	_delay_ms(1); //d
	uu_write(text, 26);
//	_delay_ms(1); //d
//	UDR0 = 'x'; //d
//	_delay_ms(1); //d
//	for (uint8_t i=0; i<TXBSZ; ++i) { //d
//		UDR0 = txb.data[i]; //d
//		_delay_ms(1); //d
//	} //d
//	UDR0 = ' '; //d
//	_delay_ms(1); //d
//	UDR0 = txb.start; //d
//	_delay_ms(1); //d
//	UDR0 = txb.length; //d
//	_delay_ms(1); //d
//	UDR0 = '\n'; //d
//	_delay_ms(1); //d
}
	for(;;) {
		LED_ON(LED0);
		_delay_ms(500);
		LED_OFF(LED0);
		_delay_ms(500);
	}
}

ISR(BAD_ISR_vect) {
	for (;;) {
		ledhello();
	}
}
