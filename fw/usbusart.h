#ifndef __USBUSART_H__
#define __USBUSART_H__

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>

#define UU_DDR DDRD
#define UU_PORT PORTD
#define UU_PIN PIND

#define UU_RTS PD7	/* o */
#define UU_CTS PD6	/* i */
#define UU_TX PD1	/* o */
#define UU_RX PD0	/* i */

#define UU_TXBSZ 8
#define UU_RXBSZ 16

#define UU_RINGBUFFER(X) struct {	\
			uint8_t data[X];		\
			uint8_t start;			\
			uint8_t length;			\
		}

#define UU_RINGBUFFER_PUT(b, v, SZ) {					\
			b.data[ (b.start+b.length) & (SZ-1) ] = v;	\
			++b.length;									\
		}

#define UU_RINGBUFFER_GET(v, b, SZ) {			\
			v = b.data[ b.start ];				\
			--b.length;							\
			b.start = (b.start+1) & (SZ-1);		\
		}

#define uu_cts() ((UU_PIN & _BV(UU_CTS)) == 0)
#define uu_set_rts() UU_PORT &= ~_BV(UU_RTS)
#define uu_reset_rts() UU_PORT |= _BV(UU_RTS)

void uu_setup(void);
void uu_write(uint8_t *src, uint8_t n);
void uu_read(uint8_t *dst, uint8_t n);

#endif
