#include "usbusart.h"

volatile UU_RINGBUFFER(UU_TXBSZ) uu_txb;
volatile UU_RINGBUFFER(UU_RXBSZ) uu_rxb;

void uu_setup(void) {
	// 1.25Mbd 8N1 @ 20MHz fcpu
	// interrupts enabled
	uu_txb.start = 0;
	uu_txb.length = 0;
	uu_rxb.start = 0;
	uu_rxb.length = 0;
	UCSR0B |= _BV(RXCIE0) | _BV(RXEN0) | _BV(TXEN0);
	UCSR0C |= _BV(UCSZ01) | _BV(UCSZ00);
	UBRR0H = 0;
	UBRR0L = 0;
	PCICR |= _BV(PCIE3);
	PCMSK3 |= _BV(PCINT30);
	UU_DDR |= _BV(UU_RTS);
	UU_DDR &= ~_BV(UU_CTS);
	uu_set_rts();
}

void uu_write(uint8_t *src, uint8_t n) {
	while (n > 0) {
		if (uu_txb.length < UU_TXBSZ) {
			ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {			
				UU_RINGBUFFER_PUT(uu_txb, *(src++), UU_TXBSZ);
			}
			--n;
		}
		if ((uu_txb.length > 0) && uu_cts()) {
			UCSR0B |= _BV(UDRIE0);
		}
	}
}

void uu_read(uint8_t *dst, uint8_t n) {
	while (n > 0) {
		if (uu_rxb.length > 0) {
			ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
				UU_RINGBUFFER_GET(*(dst++), uu_rxb, UU_RXBSZ);
			}
			--n;
		}
		if (uu_rxb.length <= UU_RXBSZ-3) {
			uu_set_rts();
		}
	}
}

ISR(USART0_RX_vect) {
	UU_RINGBUFFER_PUT(uu_rxb, UDR0, UU_RXBSZ);
	if (uu_rxb.length > UU_RXBSZ-3) {
		uu_reset_rts();
	}
}

ISR(USART0_UDRE_vect) {
	if (uu_cts() && (uu_txb.length > 0)) {
		UU_RINGBUFFER_GET(UDR0, uu_txb, UU_TXBSZ);
	}
	if (uu_txb.length < 1) {
		UCSR0B &= ~_BV(UDRIE0);
	}
}

ISR(PCINT3_vect) {
	if (uu_cts() && (uu_txb.length > 0)) {
		UCSR0A |= _BV(UDRIE0);
	}
	else {
		UCSR0A &= ~_BV(UDRIE0);
	}
}
