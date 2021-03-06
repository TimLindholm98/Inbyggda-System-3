#include <avr/io.h>
#include "ADC.h"

void adc_init(){
  ADMUX = 0x00;
  ADMUX |= (1 << REFS0) | (1 << ADLAR);    // use AVcc as the reference, 8 bit left adjust
  ADCSRA |= (1 << ADPS1) | (1 << ADPS0);    // 8 prescale
  ADCSRA |= (1 << ADEN) /*| (1 << ADIE) */; // Enable enable ADC and *Interupts*
}

uint8_t read_adc(){
  ADCSRA |= (1<<ADSC);
  while(ADCSRA & (1<<ADSC));
  return ADC;
}

/*int map_conversion(long x, long in_min, long in_max, long out_min, long out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}*/
