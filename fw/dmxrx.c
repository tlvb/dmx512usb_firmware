#define __DMXRX_C__
#include "dmxrx.h"
#include "leds.h"

volatile uint8_t dr_state;
volatile uint8_t dr_rxb[DR_RXBSZ];
volatile uint8_t dr_heartbeat;

void (*dr_ucb)(uint8_t, uint8_t);

void dr_setup(void) {
	/* 250k 8N2 */
	/* interrupt on complete reception */

	DR_DDR &= ~_BV(DR_RX);
	DR_DDR |= _BV(DR_RXEN_);
	DR_PORT &= ~_BV(DR_RXEN_);

	UBRR1H = DR_UBRRH;
	UBRR1L = DR_UBRRL;

	UCSR1B |= _BV(RXCIE1) | _BV(RXEN1);
	UCSR1C |= _BV(USBS1) | _BV(UCSZ11) | _BV(UCSZ10);
	dr_state = 255;
	dr_ucb = 0;
	dr_heartbeat = 0;

}

void dr_register_update_callback(void (*ucb)(uint8_t, uint8_t)) {
	dr_ucb = ucb;
}

ISR(USART1_RX_vect) {
	uint8_t bits = UCSR1A;
	uint8_t byte = UDR1;
	if ((bits & _BV(FE1)) != 0) {
		/* SFB */
		dr_state = DR_START;
	}
	else if (dr_state == DR_START) {
		/* ignore packet if start code != zero */
		if (byte == 0) {
			dr_state = 0;
			dr_heartbeat = 1;
		}
		else {
			dr_state = DR_IGNORE;
		}
	}
	else if (dr_state < DR_RXBSZ) {
		if (dr_ucb != 0 && byte != dr_rxb[dr_state]) {
			(*dr_ucb)(dr_state, byte);
		}
		dr_rxb[dr_state++] = byte;
	}
	else {
		dr_state = DR_IGNORE;
	}
}

