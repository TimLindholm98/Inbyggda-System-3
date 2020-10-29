#include <avr/io.h>
#include "button.h"

void button_init(){
  DDRD &= ~(1 << PD2);    // Button to input
  EICRA |= (1<<ISC01);  // Interupts for pin 2
  EIMSK |= (1 << INT0);
}
