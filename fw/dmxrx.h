#ifndef __DMXRX_H__
#define __DMXRX_H__

#include <avr/io.h>
#include <avr/interrupt.h>

#define DR_DDR DDRD
#define DR_PORT PORTD
#define DR_RX PD2
#define DR_RXEN_ PD5

#define DR_UBRRH 0
#define DR_UBRRL 4
#define DR_RXBSZ 64

#define DR_IGNORE 254
#define DR_START 255

void dr_setup(void);
void dr_register_update_callback(void (*ucb)(uint8_t, uint8_t));

#endif
