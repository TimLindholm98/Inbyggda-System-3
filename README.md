# Inbyggda-System-3

## DELUPPGIFT 1: Koppla upp följande tre kretsar på ditt kopplingsdäck

Jag valde att koppla på mitt kopplingsdäck även fast det skulle gå att göra pin-8 till en output så tror jag att det skulle funka, men är ingen expert på analoga signaler.\

![alt text](https://i.imgur.com/KrzlH5e.jpg)

## Deluppgift 2: läs av digitalingång

#### 1. Konfigurera upp pinne PD2 som ingång, som använder ett externt pulldown motstånd.
behövs inte, se bild.

#### 2. Skriv, och anropa periodiskt, en funktion som läser av pinnen och skriver ut ’pushed\r\n’ och ’released\r\n’ på UARTen när knappen trycks in, respektive släpps upp. Meddelandena ska bara skrivas ut en gång per knapptryck. 
![print once mechanism works, with debounce.. i think.... hmm](https://github.com/TimLindholm98/Inbyggda-System-3/commit/3be8de32f20fa220095559305caa3193bc270bb4) = namn på commiten och länk. 

```c
//----- main.c -----
#include "timer.h"
#include "button.h"

static volatile unsigned short timer_indexing = 0;

void main(void) {
  LED_init();
  timer_init();
  button_init();
  sei();
  while (1) {
    if (timer_indexing > 10) {
      current_state = check_button_state(last_state);
      timer_indexing = 0;
    }

    if (current_state != last_state) {
      if (current_state) {
        printf_P(PSTR("Pushed\r\n"));
      } else {
        printf_P(PSTR("Released\r\n"));
      }
    }
    last_state = current_state;
  }
}
```
```c
//----- button.c -----
#include <avr/io.h>
#include <stdbool.h>
#include "button.h"

void button_init() {
  DDRD &= ~(1 << PD2); // Button to input
}

bool check_button_state(bool last_state) {
  if (PIND & (1 << PD2)) {
    return true; // HIGH
  } else {
    return false; // LOW
  }
}
//----- timer.c -----
#include <avr/io.h>
#include "timer.h"

void timer_init() {
  OCR0A = 77; // 5 ms / 200 hz
  TCCR0A |= (1 << WGM01); // enable ctc mode
  TCCR0B |= (1 << CS02) | (1 << CS00); //Set prescaler 1024
  TIMSK0 |= (1 << OCIE0A);  // enable timer compare interrupt
}
```
 
 ## Deluppgift 3: LÄS SPÄNNING MED HJÄLP AV ADCN
 ![Deluppgift 3: LÄS SPÄNNING MED HJÄLP AV ADCN](https://github.com/TimLindholm98/Inbyggda-System-3/commit/f33fd18c83f3b07748856373f3a2c22045944ce5) allt funkar här men äldre\
 ![Deluppgift 3: LÄS SPÄNNING MED HJÄLP AV ADCN](https://github.com/TimLindholm98/Inbyggda-System-3/commit/dadcaf8d78add5aed08256a894e4039e7814d12d) liten fix men nyare
 
 #### 1. Konfigurera upp ADC-enheten mha kap 28 i databladet:
  * Single conversion
  * Bit-data, vänsterjusterat
  * Prescaler = 8, referensspänning VCC
  * Använd kanal ADC0
 
  ```c
//----- ADC.c -----
#include <avr/io.h>

#include "ADC.h"'

void adc_init() {
    ADMUX = 0x00;
    ADMUX |= (1 << REFS0) | (1 << ADLAR); // use AVcc as the reference, 8 bit left adjust
    ADCSRA |= (1 << ADPS1) | (1 << ADPS0); // 8 prescale
    ADCSRA |= (1 << ADIE) | (1 << ADEN); // Enable Interrups and enable ADC
}

int read_adc() {
    ADCSRA |= (1 << ADSC);
    while (ADCSRA & (1 << ADSC));
    return ADC;
}
```
 #### 2.Skriv en avbrottshanterare för ’ADC conversion complete’, och i den, läs ut ADC-värdet och spara undan det i en global variabel
 ```c
#include "timer.h"
#include "button.h"

volatile uint16_t adc_value = 0;
static volatile bool new_pwm_value = false;

void main(void) {
  LED_init();
  uart_init();
  timer_init0();
  timer_init2();
  //button_init();
  adc_init();

  bool last_state = false;
  bool current_state = false;

  sei();
  while (1) {
    if (new_pwm_value) {
      OCR0A = adc_value;
      new_pwm_value = false;
    }
  }
}

ISR(TIMER2_COMPA_vect) {
  ADCSRA |= (1 << ADSC);
}

ISR(ADC_vect) {
  adc_value = ADCH;
  new_pwm_value = true;
}
 ```
 #### 3. Konfigurera upp timer0 i Fast PWM-mod på samma sätt som i labb 2
  * Fast PWM-mod, 255 som TOP, prescaler = 64
```c
//----- timer.c -----

void timer_init0() {
    TCCR0A |= (1 << WGM01) | (1 << WGM00) | (1 << COM0A1);    // Timer0 Set the Fast PWN
    TCCR0B |= (1 << CS01) | (1 << CS00);    //Timer0 set prescaler to 64 and start the timer

    OCR0A = 155;
}
``` 
#### 4. Konfigurera upp timer2 i CTC-mod på samma sätt som i labb 2, men slå på interrupts för compare match, och använd inte busy wait-loopar
 * CTC-mod, 100 Hz, prescaler 1024, ’clear on match/set on bottom’
 * Skriv en avbrottshanterare för compare match A, och i den, starta en ADC-omvandling, och skriv föregående ADC-värde (sparat i variabeln) till OCR0A
 ```c
 //----- timer.c -----
 void timer_init2() {
  OCR2A = 255;
  TCNT2 = 0;
  TIMSK2 |= (1 << OCIE2A);    //Timer/Counter2, Output Compare A Match Interrupt Enable
  TCCR2A |= (1 << WGM21); // enable ctc mode, Clear OC2A on Compare Match, set OC2A at BOTTOM (non-inverting mode)
  TCCR2B |= (1 << CS22) | (1 << CS21) | (1 << CS20);  //Set prescaler
}

//----- main.c -----

ISR(TIMER2_COMPA_vect) {
  ADCSRA |= (1 << ADSC);
}

ISR(ADC_vect) {
  adc_value = ADCH;
  new_pwm_value = true;
}
 ```
#### Deluppgift 4 (VG-krav): ANVÄNDARSTYRT BETEENDE
![Working state-machine](https://github.com/TimLindholm98/Inbyggda-System-3/commit/616e8f165fa91f841017c9ee79aafee87b14e2ee)\
![fixed pulse speed](https://github.com/TimLindholm98/Inbyggda-System-3/commit/894f90444ba59161a97f2880c8ed7dafb1856d9a) den här kan du ladda upp på din atmega328p
 ```c
//----- main.c -----
#include <stdio.h>
#include <stdbool.h>
#include "state.h"
#include "led.h"
#include "serial.h"
#include "timer.h"
#include "button.h"
#include "ADC.h"

SYSTEM_STATE state;
volatile int timer_indexing = 0;

void main(void) {
  LED_init();
  timer_init2();
  button_init();
  adc_init();
  state.current_state = PULSE_MODE;
  state.change_state = false;
  state.timer_i = 0;

  sei();
  while (1) {
    if (state.change_state) {
      state.current_state = change_state();
      state.change_state = false;
    } else {
      cli();
      EXECUTER( & state);
      sei();
    }

    if (timer_indexing > 1000) {
      printf_P(PSTR("%d"), state.current_state);
      timer_indexing = 0;
    }
  }
}

ISR(TIMER2_COMPA_vect) {
  if (state.current_state == POTENTIOMETER_MODE) {
    ADCSRA |= (1 << ADSC);
  }
  check_button_state( & state);
  timer_indexing++;
  state.timer_i++;
}

ISR(ADC_vect) {
  OCR0A = ADCH;
}

ISR(TIMER0_COMPA_vect) {
  static uint8_t timer_i = 0;
  timer_i++;
  if (timer_i > 30) {
    OCR0A = simple_ramp();
  }
}

//----- ADC.c -----
#include <avr/io.h>

#include "ADC.h"'

void adc_init() {
  ADMUX = 0x00;
  ADMUX |= (1 << REFS0) | (1 << ADLAR); // use AVcc as the reference, 8 bit left adjust
  ADCSRA |= (1 << ADPS1) | (1 << ADPS0); // 8 prescale
  ADCSRA |= (1 << ADIE) | (1 << ADEN); // Enable Interrups and enable ADC
}

int read_adc() {
  ADCSRA |= (1 << ADSC);
  while (ADCSRA & (1 << ADSC));
  return ADC;
}
//----- LED.c -----

void LED_init() {
  DDRD |= (1 << PB6);
}
//----- timer.c -----

void timer_init0() {
  TCCR0A |= (1 << WGM01) | (1 << WGM00) | (1 << COM0A1); // Timer0 Set the Fast PWN
  TCCR0B |= (1 << CS01) | (1 << CS00); //Timer0 set prescaler to 64 and start the timer

  OCR0A = 155;
}
 
void timer_init2() {
  TCNT2 = 0;
  TIMSK2 |= (1 << OCIE2A); //Timer/Counter2, Output Compare A Match Interrupt Enable
  TCCR2A |= (1 << WGM21); // enable ctc mode, Clear OC2A on Compare Match, set OC2A at BOTTOM (non-inverting mode)
  TCCR2B |= (1 << CS22) | (1 << CS21) | (1 << CS20); //Set prescaler
}
//----- button.c -----
#include <avr/io.h>
#include <stdbool.h>
#include "state.h"
#include "button.h"


void check_button_state(SYSTEM_STATE * state) {
  static bool last_button_state;
  static bool new_button_state;

  if (PIND & (1 << PD2)) {
    new_button_state = true; // HIGH
  } else {
    new_button_state = false; // LOW
  }

  if (new_button_state != last_button_state) {
    if (new_button_state) {
      //printf_P(PSTR("Pushed\r\n"));
    } else {
      //printf_P(PSTR("Released\r\n"));
      state -> change_state = true;
    }
  }
  last_button_state = new_button_state;
}
//----- led.c -----

#include <avr/io.h>
#include "stdbool.h"
#include "state.h"
#include "led.h"

void LED_init() {
  DDRD |= (1 << PB6);
}

int simple_ramp() {
  static bool ramp_up = true;
  static int pwm_value = 0;

  if (ramp_up) {
    pwm_value++;
    if (pwm_value >= 255) {
      ramp_up = false;
    }
  } else {
    pwm_value--;
    if (pwm_value <= 0) {
      ramp_up = true;
    }
  }
  return pwm_value;
}

void toggle_LED_100(SYSTEM_STATE * state) {
  if (state -> timer_i >= 1000) {
    PORTD ^= (1 << PD6);
  }
}

```
