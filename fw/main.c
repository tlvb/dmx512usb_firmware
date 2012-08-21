#include "leds.h"
#include "usbusart.h"
#include "dmxrx.h"


void update(uint8_t channel, uint8_t value) {
	uint8_t bytes[2] = { channel, value };
	uu_write(bytes, 2);
}



int main(void) {
	led_setup();
	uu_setup();
	dr_setup();
	dr_register_update_callback(&update);

	led_hello();

	sei();

	for(;;);

}
