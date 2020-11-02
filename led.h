#ifndef _LED_H_
#define _LED_H_

void LED_init(void);

int simple_ramp(void);

void toggle_LED(void);
void toggle_LED_100(SYSTEM_STATE *state);

#endif // _LED_H_
