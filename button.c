#include <avr/io.h>
#include <stdbool.h>
#include "button.h"


void button_init(){
  DDRD &= ~(1 << PD2);    // Button to input
  /*EICRA |= (1 << ISC01) | (1 << ISC00);  // Interupts for pin 2
  EIMSK |= (1 << INT0);*/
}

bool check_button_state(bool last_state){
	if(PIND & (1 << PD2)){
		return true; // HIGH
	}
	else{
		return false; // LOW
	}
}
