#ifndef dc_motor
#define dc_motor

#include "pico/stdlib.h"

typedef enum{
    Right = 0,
	Left
}DC_MOTOR_CH;

void init_dc_motor(void);
void set_speed_dc_motor(DC_MOTOR_CH ch, int32_t speed);
uint16_t get_dc_motor_pwm_resolution(void);

#endif /* DC_MOTOR_H_ */
