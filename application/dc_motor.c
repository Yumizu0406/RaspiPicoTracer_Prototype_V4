#include "dc_motor.h"
#include "../driver/TB6612FNG.h"

#define SHORT_MAX 65535

/***********************************************************************************************************************
 * Function Name: init_dc_motor
 * Description  : dc_motor初期化処理
 * Arguments    : none
 * Return Value : none
 ***********************************************************************************************************************/
void init_dc_motor(void)
{
}

/***********************************************************************************************************************
 * Function Name: set_speed_dc_motor
 * Description  : モーター速度設定処理
 * Arguments    : ch モーターのチャンネル
				  speed 速度
 * Return Value : none
 ***********************************************************************************************************************/
void set_speed_dc_motor(DC_MOTOR_CH ch, int32_t speed)
{
	int32_t speed_int_abs;
	TB6612FNG_CH TB6612FNG_ch;

	if(ch == Right){
		TB6612FNG_ch = B;
	} else {
		TB6612FNG_ch = A;
	}

	if(speed < 0){
		speed_int_abs = -1 * speed;
	} else {
		speed_int_abs = speed;
	}

	if(SHORT_MAX < speed_int_abs){
		speed_int_abs = SHORT_MAX;
	}

	if(0 <= speed){
		if(ch == Right){
			set_control(TB6612FNG_ch,CW);
		} else {
			set_control(TB6612FNG_ch,CW);
		}
	} else {
		if(ch == Right){
			set_control(TB6612FNG_ch,CCW);
		} else {
			set_control(TB6612FNG_ch,CCW);
		}
	}
	set_pwm_value(TB6612FNG_ch,(uint16_t)speed_int_abs);
}

/***********************************************************************************************************************
 * Function Name: get_dc_motor_pwm_resolution
 * Description  : PWM分解能取得処理
 * Arguments    : none
 * Return Value : 分解能
 ***********************************************************************************************************************/
uint16_t get_dc_motor_pwm_resolution(void)
{
	return get_pwm_resolution();
}
