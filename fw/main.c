#include "dmxrx.h"
#include "leds.h"


#define nexa_on(x) PORTB |= _BV(x)
#define nexa_off(x) PORTB &= ~_BV(x)


volatile uint8_t nexa_target;
uint8_t nexa_state;


void update(uint8_t channel, uint8_t value) {
	if (channel == 0) {
		if (value > 0x7f)
			nexa_target |= 1;
		else 
			nexa_target &= 2;
	}
	else if (channel == 1) {
		if (value > 0x7f)
			nexa_target |= 2;
		else
			nexa_target &= 1;
	}
}



int main(void) {
	led_setup();
	dr_setup();
	dr_register_update_callback(&update);

	led_hello();
				//	 123456789abcdef012

	DDRB |= 0x0f;
	nexa_off(0);
	nexa_off(1);
	nexa_off(2);
	nexa_off(3);

	sei();

	for(;;) {
		int8_t i = -1;
		if ((nexa_target & 1) != (nexa_state & 1)) {
			if ((nexa_target & 1) != 0) {
				i = 0;
				nexa_state |= 1;
			}
			else {
				i = 1;
				nexa_state &= 2;
			}
		}
		else if ((nexa_target & 2) != (nexa_state & 2)) {
			if ((nexa_target & 2) != 0) {
				i = 2;
				nexa_state |= 2;
			}
			else {
				i = 3;
				nexa_state &= 1;
			}
		}
		if (i >= 0) {
			nexa_on(i);
			led_on(LED0);
			_delay_ms(100);
			led_off(LED0);
			_delay_ms(100);
			nexa_off(i);
		}
		


	}

}
