#ifndef STATE__H_
#define STATE__H_

typedef enum {
  PULSE_MODE,         // 1
  POTENTIOMETER_MODE, // 2
  BLINK_MODE,         // 3
  OFF_MODE            // 4
}STATES;

typedef struct{
	int current_state;
	volatile bool change_state;
  volatile int timer_i;
}SYSTEM_STATE;


extern enum STATES state_enum;

int change_state();
void EXECUTER(SYSTEM_STATE *state);

#endif
