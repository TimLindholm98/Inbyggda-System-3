#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stdio.h>
#include <stdbool.h>

#include "led.h"
#include "serial.h"
#include "timer.h"
#include "button.h"

static volatile unsigned short timer_indexing = 0;

void main (void) {
	LED_init();
	uart_init();
	timer_init();
	button_init();
	sei();
	
	bool last_state = false;
	bool current_state = false;

	while(1){
		if(timer_indexing > 10){
			current_state = check_button_state(last_state);
			timer_indexing = 0;
		}

		if(current_state != last_state){
			if(current_state){
				printf_P(PSTR("Pushed\r\n"));
			}
			else{
				printf_P(PSTR("Released\r\n"));
			}
		}
		last_state = current_state;
	}
}

ISR(TIMER0_COMPA_vect){
	timer_indexing++;
}



/*
if(print_released){
	printf_P(PSTR("Released\r\n"));
	print_released = false;
}
if((PIND & (1 << PD2)) && is_pressed == true && print_released ){
	printf_P(PSTR("Pushed\r\n"));
	is_pressed = false;
}
*/
