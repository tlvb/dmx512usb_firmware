#ifndef __USBUSART_H__
#define __USBUSART_H__

#include <avr/interrupt.h>
#include <util/atomic.h>
#include "board.h"

#define TXBSZ 8
#define RXBSZ 16

#define RINGBUFFER(X) struct {	\
			uint8_t data[X];	\
			uint8_t start;		\
			uint8_t length;		\
		}

#define RINGBUFFER_PUT(b, v, SZ) {						\
			b.data[ (b.start+b.length) & (SZ-1) ] = v;	\
			++b.length;									\
		}

#define RINGBUFFER_GET(v, b, SZ) {				\
			v = b.data[ b.start ];				\
			--b.length;							\
			b.start = (b.start+1) & (SZ-1);		\
		}

void uu_init(void);
void uu_write(uint8_t *src, uint8_t n);
void uu_read(uint8_t *dst, uint8_t n);
#endif
