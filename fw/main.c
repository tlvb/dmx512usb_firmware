#include "button.h"
#include "leds.h"

int main(void) {
	button_setup();
	led_setup();
	for (;;) {
		if (button()) {
			led_hello();
		}
		else {
			led_error();
		}
	}
}
