#include "dmxtx.h"

volatile uint8_t dmxtxstate;
volatile uint8_t dmxtxb[DMXTXBSZ];
volatile uint8_t dmxtxbi;


void dmxtx_init(void) {
		/* 250k 8N2 ...but not yet -- SFB and MAB first */
		/* usart */
	UCSR1C |= _BV(USBS1) | _BV(UCSZ11) | _BV(UCSZ10);
	UBRR1H = 0; UBRR1L = 4;
		/* SFB */
	DMXPORT &=~ _BV(DMXTX);
		/* timer */
	TIMSK0 |= _BV(OCIE0A);
	OCR0A = DMXTXSFBT;
	TCNT0 = 0;
	TCCR0B |= _BV(CS01);
	dmxtxstate = DMXTXSFBS;
	dmxtxbi = 0;
}

ISR(USART1_UDRE_vect) {
		/* transmit */
	UDR1 = dmxtxb[dmxtxbi++];
	if (dmxtxbi == DMXTXBSZ) {
			/* prepare for SFB if last */
		UCSR1B &=~ _BV(UDRIE1);
		UCSR1B |= _BV(TXCIE1);
	}
}

ISR(USART1_TX_vect) {
		/* stop transmission, generate SFB */
	UCSR1B &=~ _BV(TXCIE1) | _BV(TXEN1);
	DMXPORT &=~ _BV(DMXTX);
	OCR0A = DMXTXSFBT;
	TCNT0 = 0;
	TCCR0B |= _BV(CS01);
	dmxtxstate = DMXTXSFBS;
	dmxtxbi = 0;
}

ISR(TIMER0_COMPA_vect) {
	if (dmxtxstate == DMXTXSFBS) {
			/* SFB complete, generate MAB */
		OCR0A = DMXTXMABT;
		DMXPORT |= _BV(DMXTX);
		dmxtxstate = DMXTXMABS;
	}
	else if (dmxtxstate == DMXTXMABS) {
			/* MAB complete, stop timer, start transmission */
		TCCR0B &=~ _BV(CS02) | _BV(CS01) | _BV(CS00);
		UCSR1B |= _BV(UDRIE1) | _BV(TXEN1);
		UDR1 = 0;
		dmxtxstate = DMXTXTXS;
	}
}
