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

		/* enable reception, */
		/* disable transmission */
	DMXDDR |= _BV(DMXREN) | _BV(DMXTEN);
	DMXPORT &= ~(_BV(DMXREN) | _BV(DMXTEN));

		/* 1.25Mbd 8N1 */
	UCSR0B |= _BV(TXEN0);
	UCSR0C |= _BV(UCSZ01) | _BV(UCSZ00);
	UBRR0H = 0;
	UBRR0L = 0;
	
	while (!button());

	led_hello();

	asm volatile(
			"															\
			push	r16												\n\t\
			push	r17												\n\t\
			push	r18												\n\t\
			ldi		r18,		8			; 1l		10l			\n\t\
			ldi		r17,		0			; 1l		11l			\n\t\
																		\
loop_start:	in		r16,		%0			; 1			1			\n\t\
			lsr		r16						; 1			2			\n\t\
			lsr		r16						; 1			3			\n\t\
			andi	r16,		1			; 1			4			\n\t\
			or		r17,		r16			; 1			5			\n\t\
			dec		r18						; 1			6			\n\t\
			brne	not_last				; 1l/2n		7l/8n		\n\t\
																		\
last:		sts		%1,			r17			; 2l		9l			\n\t\
			ldi		r18,		8			; 1l		10l			\n\t\
			ldi		r17,		0			; 1l		11l			\n\t\
			rjmp	loop_end				; 2l		13l			\n\t\
																		\
not_last:	lsl		r17						; 1l		9n			\n\t\
			nop								; 1l		10n			\n\t\
			nop								; 1l		11n			\n\t\
			nop								; 1l		12n			\n\t\
			nop								; 1l		13n			\n\t\
																		\
loop_end:	nop								; 1			14			\n\t\
			rjmp	loop_start				; 2			16			\n\t\
																		\
			pop		r18												\n\t\
			pop		r17												\n\t\
			pop		r16												\n\t\
			"
			:
			: "I" (_SFR_IO_ADDR(PIND)),
				"" (UDR0)
	);

	for(;;);

}
