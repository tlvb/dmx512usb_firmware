#include "board.h"
#include "leds.h"

int main(void) {
	leds_setup();
	led_hello();
	dmxtx_setup();
	/* TODO: stuff */
	for(;;);
}
