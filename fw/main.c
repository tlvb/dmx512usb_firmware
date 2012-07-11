#include "leds.h"

int main(void) {
	led_setup();

	for(;;) {
		led_hello();
		_delay_ms(1000);
		led_error();
		_delay_ms(1000);
	}
}
