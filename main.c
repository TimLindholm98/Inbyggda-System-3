#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stdio.h>
#include <stdbool.h>

#include "led.h"
#include "serial.h"
#include "timer.h"
#include "button.h"

volatile bool is_pressed = true;
volatile bool print_released = false;

void main (void) {
	LED_init();
	uart_init();
	timer_init();
	button_init();
	sei();

	while(1){
			if((PIND & (1 << PD2)) && is_pressed == true && print_released ){
					printf_P(PSTR("Pushed\r\n"));
					is_pressed = false;
			}
			if(print_released){

				printf_P(PSTR("Released\r\n"));
				print_released = false;
			}
	}
}

ISR(INT0_vect){
	is_pressed = true;
	print_released = true;
}


/*

*/
