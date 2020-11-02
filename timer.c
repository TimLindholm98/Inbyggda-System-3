#include <avr/io.h>
#include "timer.h"

void timer_init0() {
  TCCR0A |= (1 << WGM01) | (1 << WGM00) | (1 << COM0A1);
  TCCR0B |= (1 << CS00) | (1 << CS01);  //Set prescaler
  TIMSK0 |= (1 << OCIE0A); // sätter på interrupts för  TIMER0_COMPA
}

void timer_init2() {
  TCNT2 = 0;

  TCCR2A |= (1 << WGM21); // enable ctc mode, Clear OC2A on Compare Match, set OC2A at BOTTOM (non-inverting mode)
  TCCR2B |= (1 << CS22) | (1 << CS21) | (1 << CS20);  //Set prescaler
  TIMSK2 |= (1 << OCIE2A); //Timer/Counter2, Output Compare A Match Interrupt Enable
}
