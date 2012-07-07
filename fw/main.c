#include "board.h"

void waitusart(void) {
	LED_ON(LED0);
	while ((UCSR0A&_BV(UDRE0))==0);
	LED_OFF(LED0);
	LED_ON(LED1);
	while ((USBPIN & _BV(USBCTS)) != 0);
	LED_OFF(LED1);
}

void rgb_from_index(uint8_t *rgb, uint16_t index) {
	while (index >= 1536) {
		index -= 1536;
	}
	uint8_t i = index & 0xff;
	if (index < 256) {
		rgb[0] = 255;	rgb[1] = i;		rgb[2] = 0;
	}
	else if (index < 512) {
		rgb[0] = 255-i;	rgb[1] = 255;	rgb[2] = 0;
	}
	else if (index < 768) {
		rgb[0] = 0;		rgb[1] = 255;	rgb[2] = i;
	}
	else if (index < 1024) {
		rgb[0] = 0;		rgb[1] = 255-i;	rgb[2] = 255;
	}
	else if (index < 1280) {
		rgb[0] = i;		rgb[1] = 0;		rgb[2] = 255;
	}
	else if (index < 1536) {
		rgb[0] = 255;	rgb[1] = 0;		rgb[2] = 255-i;
	}
}

int main(void) {
	uint8_t rgb[3];

	pinsetup();
	ledhello();

	UCSR0B |= _BV(RXEN0) | _BV(TXEN0);
	UCSR0C |= _BV(UCSZ01) | _BV(UCSZ00);
	UBRR0H = 0; UBRR0L = 0;

	for (char *t="INIT\n"; *t!=0; ++t) {
		waitusart();
		UDR0 = *t;
	}
	for(uint16_t y=0; y<1536; ++y) {
		for(uint16_t x=0; x<1536; ++x) {
			rgb_from_index(rgb, (x^y));
			waitusart();
			UDR0 = rgb[0];
			waitusart();
			UDR0 = rgb[1];
			waitusart();
			UDR0 = rgb[2];
		}
	}
	for (char *t="DONE\n"; *t!=0; ++t) {
		waitusart();
		UDR0 = *t;
	}
	LED_ON(LED2);
	for (;;);
	

}
