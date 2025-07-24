/***********************************************************************************************************************
* ファイル名 : control.c
* 説明 : 制御モジュール
***********************************************************************************************************************/

/***********************************************************************************************************************
include
***********************************************************************************************************************/
#include "control.h"
#include "../driver/lineSensor.h"
#include "dc_motor.h"
#include "battery_monitor.h"
#include <stdint.h>

/***********************************************************************************************************************
define and const
***********************************************************************************************************************/
#define TREAD_WIDTH_MM	90
#define HALF_TREAD_WIDTH_MM TREAD_WIDTH_MM/2

/***********************************************************************************************************************
global
***********************************************************************************************************************/
static control_status_t control_status;
static run_mode_t run_mode;
static control_parameters_t parameters[run_mode_num];
static int32_t pwm_resolution;
static uint16_t stop_timer;
/***********************************************************************************************************************
prototype
***********************************************************************************************************************/
void set_motor_control(int32_t speed,int16_t angular);
uint32_t calculate_pwm_duty_value_from_vattery_voltage_mV(uint16_t aim_voltage_mV);
/***********************************************************************************************************************
 * Function Name: init_control
 * Description  : 制御処理初期化処理
 * Arguments    : none
 * Return Value : none
 ***********************************************************************************************************************/
void init_control(void)
{
    control_status = stop;
    run_mode = trial_run;
    pwm_resolution = get_dc_motor_pwm_resolution();
    stop_timer = 0u;
}

/***********************************************************************************************************************
 * Function Name: set_control_parameter
 * Description  : パラメータ設定処理
 * Arguments    : none
 * Return Value : none
 ***********************************************************************************************************************/
void set_control_parameter(run_mode_t mode, control_parameters_t parameter)
{
	parameters[mode] = parameter;
}

/***********************************************************************************************************************
 * Function Name: set_run_mode
 * Description  : 走行モード設定処理
 * Arguments    : none
 * Return Value : none
 ***********************************************************************************************************************/
void set_run_mode(run_mode_t run_mode_)
{
	run_mode = run_mode_;
}

/***********************************************************************************************************************
 * Function Name: update_control
 * Description  : 制御処理更新処理
 *                1ms周期で更新されることを想定
 * Arguments    : none
 * Return Value : none
 ***********************************************************************************************************************/
void update_control(void)
{
    int16_t line_center_deff = get_line_center_deff();
    int16_t ang_control = line_center_deff * (-1);
    int32_t pwm_value_at_straight;
    uint8_t goalmarker_count = get_goal_marker_count();

    update_battery();

    pwm_value_at_straight = calculate_pwm_duty_value_from_vattery_voltage_mV(parameters[run_mode].speed_at_straight);

    if(control_status == switching_to_run){
        clear_goal_marker_count();
        control_status = run;
    } else if(control_status == run){
        set_motor_control(pwm_value_at_straight, ang_control);
        if(2 <= goalmarker_count){
            control_status = switching_to_stop;
            stop_timer = 600u;
        }
    } else if(control_status == switching_to_stop){
        set_motor_control(pwm_value_at_straight, ang_control);
        if(stop_timer == 0u){
            control_status = stop;
        }
    } else if(control_status == stop){
        set_motor_control(1, 0);
    }

    if(0u < stop_timer){
        stop_timer--;
    }
}

/***********************************************************************************************************************
 * Function Name: set_control_status
 * Description  : 制御状態設定処理
 * Arguments    : none
 * Return Value : none
 ***********************************************************************************************************************/
void set_control_status(control_status_t status)
{
    control_status = status;
}

/***********************************************************************************************************************
 * Function Name: get_control_status
 * Description  : 制御状態取得処理
 * Arguments    : none
 * Return Value : none
 ***********************************************************************************************************************/
control_status_t get_control_status(void)
{
    return control_status;
}

/***********************************************************************************************************************
 * Function Name: set_motor_control
 * Description  : モーターコントロール設定処理
 * Arguments    : none
 * Return Value : none
 ***********************************************************************************************************************/
void set_motor_control(int32_t speed,int16_t angular)
{
	int control_speed_value_R;
	int control_speed_value_L;

	control_speed_value_R = speed + HALF_TREAD_WIDTH_MM * (int32_t)angular;
	control_speed_value_L = speed - HALF_TREAD_WIDTH_MM * (int32_t)angular;

    set_speed_dc_motor(Right,control_speed_value_R);
    set_speed_dc_motor(Left, control_speed_value_L);
}

/***********************************************************************************************************************
 * Function Name: calculate_pwm_duty_value_from_vattery_voltage_mV
 * Description  : 目標電圧値に応じたpwm値をバッテリー電圧値から計算する
 * Arguments    : aim_voltage_mV 目標電圧値
 * Return Value : none
 ***********************************************************************************************************************/
uint32_t calculate_pwm_duty_value_from_vattery_voltage_mV(uint16_t aim_voltage_mV)
{
	uint32_t result;
	uint16_t vattery_voltage_mV;
	uint32_t tmp_result;

	vattery_voltage_mV = get_battery_voltage_mV();

	tmp_result = (uint32_t)pwm_resolution * (uint32_t)aim_voltage_mV;
	result = tmp_result / (uint32_t)vattery_voltage_mV;

	return result;
}
