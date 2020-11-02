#include <avr/io.h>
#include "stdbool.h"
#include "state.h"
#include "led.h"

void LED_init() {
	DDRD |= (1 << PB6);
}

int simple_ramp(){
  static bool ramp_up = true;
  static int pwm_value = 0;

  if(ramp_up){
    pwm_value++;
    if(pwm_value >= 255){
      ramp_up = false;
    }
  }
  else{
    pwm_value--;
    if(pwm_value <= 0){
      ramp_up = true;
    }
  }
  return pwm_value;
}

void toggle_LED(){
	PORTB ^= (1 << PB6);
}

void toggle_LED_100(SYSTEM_STATE *state){
	if (state->timer_i >= 1000) {
		PORTD ^= (1 << PD6);
	}
}
