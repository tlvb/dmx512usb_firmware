#include "usbusart.h"

volatile RINGBUFFER(TXBSZ) txb;
volatile RINGBUFFER(RXBSZ) rxb;

void uu_init(void) {
	// 1.25Mbd 8N1 @ 20MHz fcpu
	// interrupts enabled
	txb.start = 0;
	txb.length = 0;
	rxb.start = 0;
	rxb.length = 0;
	UCSR0B |= _BV(RXCIE0) | _BV(RXEN0) | _BV(TXEN0);
	UCSR0C |= _BV(UCSZ01) | _BV(UCSZ00);
	UBRR0H = 0;
	UBRR0L = 0;
	PCICR |= _BV(PCIE3);
	PCMSK3 |= _BV(PCINT30);

}

void uu_write(uint8_t *src, uint8_t n) {
	while (n > 0) {
		if (txb.length < TXBSZ) {
			ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {			
				RINGBUFFER_PUT(txb, *(src++), TXBSZ);
			}
			--n;
		}
		if ((txb.length > 0) && ((USBPIN & _BV(USBCTS)) == 0)) {
			UCSR0B |= _BV(UDRIE0);
		}
	}
}

void uu_read(uint8_t *src, uint8_t n) {
}

ISR(USART0_RX_vect) {
}

ISR(USART0_UDRE_vect) {
	if (((USBPIN & _BV(USBCTS)) == 0) && (txb.length > 0)) {
		RINGBUFFER_GET(UDR0, txb, TXBSZ);
	}
	if (txb.length < 1) {
		UCSR0B &=~ _BV(UDRIE0);
	}
}

ISR(PCINT3_vect) {
	if (((USBPIN & _BV(USBCTS)) == 0) && (txb.length > 0)) {
		UCSR0A |= _BV(UDRIE0);
		LED_ON(LED1);
	}
	else {
		LED_ON(LED2);
		UCSR0A &=~ _BV(UDRIE0);
	}
}
