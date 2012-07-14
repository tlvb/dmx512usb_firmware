#include <avr/io.h>
#include <leds.h>
#include <button.h>

#define DMXDDR DDRD
#define DMXPORT PORTD
#define DMXPIN PIND

#define DMXREN PD5	/* o */
#define DMXTEN PD4	/* o */
#define DMXTX PD3	/* o */
#define DMXRX PD2	/* i */

int main(void) {
	
	led_setup();
	button_setup();
	led_hello();

		/* enable reception, REN is active low*/
		/* disable transmission, TEN is active high */
	DMXDDR |= _BV(DMXREN) | _BV(DMXTEN);
	DMXDDR &= ~_BV(DMXRX);
	DMXPORT &= ~(_BV(DMXREN) | _BV(DMXTEN));

		/* 1.25Mbd 8N1 */
	UCSR0B |= _BV(TXEN0);
	UCSR0C |= _BV(UCSZ01) | _BV(UCSZ00);
	UBRR0H = 0;
	UBRR0L = 0;
	
	while (!button());

	led_hello();

	/*	r16 -	samples portd 
		r17 -	keeps the sampled data bits
		r18	-	counts down until 8 bits have been sampled
				and it is time to send
	*/

	asm volatile(
			"															\
			push	r16												\n\t\
			push	r17												\n\t\
			push	r18												\n\t\
			ldi		r18,		8			; 1						\n\t\
			ldi		r17,		0			; 1						\n\t\
																		\
start_%=:	in		r16,		%[pind]		; 1			1			\n\t\
			bst		r16,		2			; 1			2			\n\t\
			bld		r17,		0			; 1			3			\n\t\
			dec		r18						; 1			4			\n\t\
			brne	nosend_%=				; 1/2		5/6			\n\t\
																		\
send_%=:	sts		%[udr0],	r17			; 2			7			\n\t\
			ldi		r18,		8			; 1			8			\n\t\
			rjmp	again_%=				; 2			10			\n\t\
																		\
nosend_%=:	lsl		r17						; 1			7			\n\t\
			nop								; 1			8			\n\t\
			nop								; 1			9			\n\t\
			nop								; 1			10			\n\t\
																		\
again_%=:	nop								; 1			11			\n\t\
			nop								; 1n		12			\n\t\
			nop								; 1n		13			\n\t\
			nop								; 1n		14			\n\t\
			rjmp	start_%=				; 2			16			\n\t\
																		\
			pop		r18												\n\t\
			pop		r17												\n\t\
			pop		r16												\n\t\
			"
			:
			: [pind] "I" (_SFR_IO_ADDR(PIND)), [udr0] "" (UDR0)
	);

	for(;;);

}
