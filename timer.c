#include <avr/io.h>
#include "timer.h"

void timer_init() {
  // 5 ms / 200 hz
  OCR0A = 77;
  // enable ctc mode
  TCCR0A |= (1 << WGM01);
  //Set prescaler 1024
  TCCR0B |= (1 << CS02) | (1 << CS00);
  // enable timer compare interrupt
  TIMSK0 |= (1 << OCIE0A);
}
