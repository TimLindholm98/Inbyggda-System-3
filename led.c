#include <avr/io.h>
#include "led.h"

void LED_init() {
	DDRD |= (1 << PB6);
}

uint8_t simple_ramp() {
	// ...
}
