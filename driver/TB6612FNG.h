#ifndef TB6612FNG
#define TB6612FNG

#include "pico/stdlib.h"

typedef enum{
	SHORT_BRAKE = 0,
    CW,
	CCW,
	STOP
}TB6612FNG_CONTROL;

typedef enum{
    A = 0,
	B
}TB6612FNG_CH;

void init_TB6612FNG(void);
void set_pwm_value(TB6612FNG_CH ch, uint16_t value);
void set_control(TB6612FNG_CH ch, TB6612FNG_CONTROL cw_ccw);
uint16_t get_pwm_resolution(void);

#endif /* DRIVER_TB6612FNG_H_ */
