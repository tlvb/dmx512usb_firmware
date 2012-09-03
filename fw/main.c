#include "leds.h"
#include "usbusart.h"
#include "dmxrx.h"

volatile uint8_t cb_heartbeat;

void update(uint8_t channel, uint8_t value) {
	cb_heartbeat = 1;
	uint8_t bytes[2] = { channel, value };
	uu_write(bytes, 2);
}



int main(void) {
	led_setup();
	uu_setup();
	dr_setup();
	dr_register_update_callback(&update);
	cb_heartbeat = 0;

	led_hello();
				//	 123456789abcdef012
	uint8_t msg[] = "receiver firmware\n";


	sei();

	uu_write(msg, 0x12);

	for(;;) {
		if (dr_heartbeat != 0 || cb_heartbeat != 0 ) {
			uint8_t copy = dr_heartbeat | (cb_heartbeat << 1);
			dr_heartbeat = cb_heartbeat = 0;
			if (copy & 1)
				led_on(LED0);
			if (copy & 2)
				led_on(LED1);
			_delay_ms(10);
			led_off(LED0);
			led_off(LED1);
			_delay_ms(490);
		}
	}

}
