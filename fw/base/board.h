#ifndef __BOARD_H__
#define __BOARD_H__

#define F_CPU 20000000
#include <avr/io.h>
#include <stdint.h>

#define DMXPORT PORTD
#define DMXPIN PIND
#define USBPORT PORTD

#define USBRTS PD7	/* i */
#define USBCTS PD6	/* o */
#define DMXREN PD5	/* o */
#define DMXTEN PD4	/* o */
#define DMXTX PD3	/* o */
#define DMXRX PD2	/* i */
#define USBTX PD1	/* o */
#define USBRX PD0	/* i */

#define LED2 PB7
#define LED1 PB6
#define LED0 PB5

#define LEDS_ON PORTB &=~ _BV(LED2)|_BV(LED1)|_BV(LED0)
#define LEDS_OFF PORTB |= _BV(LED2)|_BV(LED1)|_BV(LED0)
#define LED_ON(2) PORTB &=~ _BV(LED2)
#define LED_ON(1) PORTB &=~ _BV(LED1)
#define LED_ON(0) PORTB &=~ _BV(LED0)
#define LED_OFF(2) PORTB |= _BV(LED2)
#define LED_OFF(1) PORTB |= _BV(LED1)
#define LED_OFF(0) PORTB |= _BV(LED0)

void pinsetup(void);
void ledhello(void);


#endif
