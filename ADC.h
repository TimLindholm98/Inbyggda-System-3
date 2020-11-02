#ifndef ADC_H_
#define ADC_H_

void adc_init();
uint8_t read_adc();
int map_conversion(long x, long in_min, long in_max, long out_min, long out_max) ;

#endif
