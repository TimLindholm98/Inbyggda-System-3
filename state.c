#include <avr/io.h>
#include <stdbool.h>
#include <stdio.h>
#include "state.h"
#include "led.h"


int change_state(){
  static uint8_t state_index = 0;
  state_index++;
  if(state_index > 3){
    state_index = 0;
  }
  switch (state_index) {
    case PULSE_MODE:
        TCCR0A |= (1 << COM0A1);
        TIMSK0 |= (1 << OCIE0A); // sätter på interrupts för  TIMER0_COMPA
        return PULSE_MODE;
    case POTENTIOMETER_MODE:
        TIMSK0 &= ~(1 << OCIE0A);
        ADCSRA |= (1 << ADIE);  // Enable ADC_vect
        return POTENTIOMETER_MODE;
    case BLINK_MODE:
        OCR0A = 0;
        ADCSRA &= ~(1 << ADIE); // Disable ADC_vect
        TCCR0A &= ~(1 << COM0A1); //Normal port operation, OC0A disconnected.
        return BLINK_MODE;
    case OFF_MODE:
        OCR0A = 0;
        PORTD &= ~(1 << PD6);   // Turn off LED
        return OFF_MODE;
    default:
        return PULSE_MODE;
  }
}

void EXECUTER(SYSTEM_STATE *state){
  switch (state->current_state) {
    case PULSE_MODE:
      break;
    case POTENTIOMETER_MODE:
      break;
    case BLINK_MODE:
      toggle_LED_100(state);
      //toggle_LED();
      break;
    case OFF_MODE:

      break;
    default:
      break;
  }
}
