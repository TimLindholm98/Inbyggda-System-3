#include <avr/io.h>
#include "timer.h"

void timer_init0() {
    TCCR0A |= (1 << WGM01) | (1 << WGM00) | (1 << COM0A1);    // Timer0 Set the Fast PWN
    TCCR0B |= (1 << CS01) | (1 << CS00);    //Timer0 set prescaler to 64 and start the timer

    OCR0A = 155;
}

void timer_init2() {
  OCR2A = 255;
  TCNT2 = 0;
  TIMSK2 |= (1 << OCIE2A);    //Timer/Counter2, Output Compare A Match Interrupt Enable
  TCCR2A |= (1 << WGM21); // enable ctc mode, Clear OC2A on Compare Match, set OC2A at BOTTOM (non-inverting mode)
  TCCR2B |= (1 << CS22) | (1 << CS21) | (1 << CS20);  //Set prescaler
}
