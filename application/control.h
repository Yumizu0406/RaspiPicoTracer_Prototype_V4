/***********************************************************************************************************************
* ファイル名 : control.h
* 説明 : 制御モジュールのヘッダファイル
***********************************************************************************************************************/

#ifndef control
#define control

/***********************************************************************************************************************
include
***********************************************************************************************************************/
#include "pico/stdlib.h"

/***********************************************************************************************************************
typedef
***********************************************************************************************************************/
typedef enum{
    stop,
	switching_to_run,
	run,
	switching_to_stop
}control_status_t;

typedef enum{
	low_speed_trial_run = 0,
	trial_run,
	normal_run,
	high_speed_run,
	run_mode_num
}run_mode_t;

typedef struct{
	uint16_t speed_at_straight;
	uint16_t speed_at_curve;
	float p_gain_at_straight;
	float p_gain_at_curve;
}control_parameters_t;

/***********************************************************************************************************************
prototype
***********************************************************************************************************************/
void init_control(void);
void set_control_parameter(run_mode_t mode, control_parameters_t parameter);
void set_run_mode(run_mode_t run_mode_);
void update_control(void);
void set_control_status(control_status_t status);
control_status_t get_control_status(void);

#endif