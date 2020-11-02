#include <avr/io.h>
#include <stdbool.h>
#include "state.h"
#include "button.h"


void button_init(){
  DDRD &= ~(1 << PD2);    // Button to input
  /*EICRA |= (1 << ISC01) | (1 << ISC00);  // Interupts for pin 2
  EIMSK |= (1 << INT0);*/
}

void check_button_state(SYSTEM_STATE *state){
  static bool last_button_state;
  static bool new_button_state;

	if(PIND & (1 << PD2)){
		new_button_state = true; // HIGH
	}
	else{
		new_button_state = false; // LOW
	}

  if(new_button_state != last_button_state){
		if(new_button_state){
			//printf_P(PSTR("Pushed\r\n"));
		}
		else{
			//printf_P(PSTR("Released\r\n"));
      state->change_state = true;
		}
	}
  last_button_state = new_button_state;
}
