#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stdio.h>
#include <stdbool.h>

#include "led.h"
#include "serial.h"
#include "timer.h"
#include "button.h"

volatile uint16_t adc_value = 0;
static volatile bool new_pwm_value = false;

void main (void) {
	LED_init();
	uart_init();
	timer_init0();
	timer_init2();
	//button_init();
	adc_init();

	bool last_state = false;
	bool current_state = false;

	sei();
	while(1){
		if(new_pwm_value){
			OCR0A = adc_value;
			new_pwm_value = false;
		}
	}
}

ISR(TIMER2_COMPA_vect){
	ADCSRA |= (1<<ADSC);
}



ISR(ADC_vect){
	adc_value = ADCH;
	new_pwm_value = true;
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
