#ifndef ADC_H
#define ADC_H

void potentiometer_init();
int read_potentiometer();
int potentiometer_conversion(long x, long in_min, long in_max, long out_min, long out_max) ;

#endif
