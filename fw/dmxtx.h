#ifndef __DMXTX_H__
#define __DMXTX_H__

#include <avr/io.h>
#include <avr/interrupt.h>

#define DT_DDR DDRD
#define DT_PORT PORTD

#define DT_TXEN PD4	/* o */
#define DT_TX PD3	/* o */

#define DT_BAUDH 0
#define DT_BAUDL 4
#define DT_PRESC _BV(CS01)

#define DT_SFBS 0
#define DT_MABS 1
#define DT_TXS 2

#define DT_TXBSZ 512
#define DT_TXBI_T uint16_t

#define DT_SFBT 240
#define DT_MABT 30

void dmxtx_setup(void);

#endif
