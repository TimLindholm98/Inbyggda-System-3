#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stdio.h>
#include <stdbool.h>

#include "state.h"
#include "led.h"
#include "serial.h"
#include "timer.h"
#include "button.h"
#include "ADC.h"


SYSTEM_STATE state;
volatile int timer_indexing = 0;

void main (void) {
	LED_init();
	uart_init();
	timer_init0();
	timer_init2();
	button_init();
	adc_init();
	state.current_state = PULSE_MODE;
	state.change_state = false;
	state.timer_i = 0;


	sei();
	while(1){
		if(state.change_state){
			state.current_state = change_state();
			state.change_state = false;
		}
		else{
			cli();
			EXECUTER(&state);
			sei();
		}

		if(timer_indexing > 1000){
			printf_P(PSTR("%d"), state.current_state);
			timer_indexing = 0;
		}
	}
}

ISR(TIMER2_COMPA_vect){
	if(state.current_state == POTENTIOMETER_MODE){
		ADCSRA |= (1<<ADSC);
	}
	check_button_state(&state);
	timer_indexing++;
	state.timer_i++;
}

ISR(ADC_vect){
	OCR0A = ADCH;
}

ISR(TIMER0_COMPA_vect){
	static uint8_t timer_i = 0;
	timer_i++;
	if(timer_i > 5){
		OCR0A = simple_ramp();
		timer_i = 0;
	}
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
