#include "board.h"
#include <util/delay.h>

void isoliotest(void) {
	for (uint8_t i=0; i<4; ++i) {
		if ((i&2) != 0) {
			/* receiver on, active low */
			LED_ON(LED2);
			DMXPORT &=~ _BV(DMXREN);
		}
		else {
			/* receiver off */
			LED_OFF(LED2);
			DMXPORT |= _BV(DMXREN);
		}
		if ((i&1) != 0) {
			/* transmitter on, active high */
			LED_ON(LED1);
			DMXPORT |= _BV(DMXTEN);
		}
		else {
			/* transmitter off */
			LED_OFF(LED1);
			DMXPORT &=~ _BV(DMXTEN);
		}
		for (uint8_t j=0; j<20; ++j) {
			DMXPORT &=~ ((j&1)<<DMXTX);
			DMXPORT |= ((j&1)<<DMXTX);

			if ((DMXPIN & _BV(DMXRX)) != 0)
				LED_ON(LED0);
			else
				LED_OFF(LED0);
			_delay_ms(50);
		}
	}
}


int main(void) {
	pinsetup();
	ledhello();
	for(;;)
		isoliotest();
}
