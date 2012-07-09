#include "board.h"
#include "usbusart.h"
#include <string.h>

int main(void) {

	pinsetup();
	uu_init();
	ledhello();
	sei();

	uint8_t *text = (uint8_t*)"ABCD\n";
	uu_write(text, 5);
	ledhello();
	for(;;);
}
