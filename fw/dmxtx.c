#define __DMXTX_C__
#include "dmxtx.h"
#include "leds.h"

volatile uint8_t dt_state;
volatile uint8_t dt_txb[DT_TXBSZ];
volatile uint8_t dt_txbi;
volatile uint8_t dt_heartbeat;

void dmxtx_setup(void) {
		/* 250k 8N2 ...but not yet -- SFB and MAB first */
		/* usart */
	UCSR1C |= _BV(USBS1) | _BV(UCSZ11) | _BV(UCSZ10);
	UBRR1H = DT_BAUDH;
	UBRR1L = DT_BAUDL;
		/* SFB */
	DT_DDR |= _BV(DT_TXEN) | _BV(DT_TX);
	DT_PORT |= _BV(DT_TXEN);
	DT_PORT &= ~_BV(DT_TX);
		/* timer */
	TIMSK0 |= _BV(OCIE0A);
	OCR0A = DT_SFBT;
	TCNT0 = 0;
	TCCR0B |= DT_PRESC;
	dt_state = DT_SFBS;
	dt_txbi = 0;
}

ISR(USART1_UDRE_vect) {
		/* transmit */
	UDR1 = dt_txb[dt_txbi++];
	if (dt_txbi == DT_TXBSZ) {
			/* prepare for SFB if last */
		UCSR1B &= ~_BV(UDRIE1);
		UCSR1B |= _BV(TXCIE1);
	}
}

ISR(USART1_TX_vect) {
		/* stop transmission, generate SFB */
	UCSR1B &= ~(_BV(TXCIE1) | _BV(TXEN1));
	DT_PORT &= ~_BV(DT_TX);
	OCR0A = DT_SFBT;
	TCNT0 = 0;
	TCCR0B |= DT_PRESC;
	dt_state = DT_SFBS;
	dt_txbi = 0;
}

ISR(TIMER0_COMPA_vect) {
	if (dt_state == DT_SFBS) {
			/* SFB complete, generate MAB */
		OCR0A = DT_MABT;
		DT_PORT |= _BV(DT_TX);
		dt_state = DT_MABS;
		led_on(LED2);
	}
	else if (dt_state == DT_MABS) {
			/* MAB complete, stop timer, start transmission */
		TCCR0B &= ~(_BV(CS02) | _BV(CS01) | _BV(CS00));
		UCSR1B |= _BV(UDRIE1) | _BV(TXEN1);
		UDR1 = 0;
		dt_heartbeat = 1;
		dt_state = DT_TXS;
	}
}
