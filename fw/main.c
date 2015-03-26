#include "dmxtx.h"
#include "button.h"
#include "leds.h"
#include <avr/io.h>

extern volatile uint8_t dt_txb[DT_TXBSZ];
extern uint8_t _end;
extern uint8_t __stack;

int main(void) {
	led_setup();
	button_setup();
	led_hello();

	// 1.25MBD @ 20MHz non interrupt based setup
	// odd parity
	UCSR0B |= _BV(RXEN0) | _BV(TXEN0);
	UCSR0C |= _BV(UPM01) | _BV(UPM00) | _BV(UCSZ01) | _BV(UCSZ00);
	UBRR0H = 0;
	UBRR0L = 0;

	uint8_t rec = 0;
	uint8_t rstate = 0;
	uint8_t mstate = 0;
	uint16_t chan = 0;
	uint16_t tick = 0;
	uint8_t val = 0;

	for (DT_TXBI_T i=0; i<DT_TXBSZ; ++i) {
		dt_txb[i] = i;
	}

	dmxtx_setup();
	sei();

	for (;;) {
		if ((UCSR0A & (_BV(FE0)|_BV(DOR0)|_BV(UPE0))) != 0) {
			// HW REPORTS BAD TRANSMISSION
			mstate = 0x00;
			led_on(LED2);
			tick = 0;
			rec = UDR0;
		}
		else if ((UCSR0A & _BV(RXC0)) != 0) { /*{{{*/
			rec = UDR0;
			rstate = rec & 0xC0;
			rec &= 0x3F;
			if (rstate == mstate) {
				switch (mstate) {
					case 0x00: // [00]CC CCCC
						chan = rec << 3;
						val = 0;
						mstate = 0x40;
						break;
					case 0x40: // [01]0C CCVV
						chan |= rec >> 2;
						val = (rec & 0x03)<<6;
						mstate = 0xC0;
						break;
					case 0xC0: // [11]VV VVVV
						val |= rec;
						dt_txb[chan & (DT_TXBSZ-1)] = val;
						if ((UCSR0A & _BV(UDRE0)) != 0) {
							UDR0 = chan&255;
						}
						led_toggle(LED0);
						mstate = 0x00;
						break;
					default:
						// INVALID STATE
						mstate = 0x00;
						led_on(LED2);
						tick = 0;
						break;
				}
			}
			else {
				// MISMATCHED STATE
				led_on(LED2);
				tick = 0;
				mstate = 0x00;
			}
			if (++tick == 0) {
				led_off(LED2);
			}
		} /*}}}*/
		if (button()) {
			// responsiveness test function, or something like that...
			led_on(LED2);
			tick = 0x7fff;
		}
	}
}
