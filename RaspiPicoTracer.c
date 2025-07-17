#include <stdint.h>
#include <stdio.h>
#include "pico/stdlib.h"
#include "driver/indicators.h"
#include "driver/cycle.h"
#include "driver/sw.h"
#include "driver/lineSensor.h"
#include "driver/adchange.h"
#include "driver/TB6612FNG.h"
#include "application/control.h"
#include "application/battery_monitor.h"
#include "application/dc_motor.h"

typedef enum{
    selecting_menu = 0,
	exe_menu
}menu_status_t;

void init_driver(void);
void menu_no_0(void);
void menu_no_1(void);
void menu_no_2(void);
void menu_no_3(void);
void menu_no_4(void);
void menu_no_5(void);
void menu_no_6(void);
void menu_no_7(void);
void exe_select_func(uint8_t select_func);

static uint8_t menu_no;
static menu_status_t menu_status;

void (*ptr_f[8])(void) = {
    menu_no_0,
    menu_no_1,
    menu_no_2,
    menu_no_3,
    menu_no_4,
    menu_no_5,
    menu_no_6,
    menu_no_7
};

int main()
{
    control_parameters_t parameter;

    menu_no = 0;
    menu_status = selecting_menu;

    stdio_init_all();
    init_battery();
    init_control();

    parameter.speed_at_straight = 500;
    parameter.p_gain_at_straight = 15.0;
    set_control_parameter(trial_run, parameter);

    init_driver();
    set_speed_dc_motor(A, 1);
    set_speed_dc_motor(B, 1);

    while (true) {
        if(menu_status == selecting_menu){
            set_Raspberry_Pi_LED_mode(led_mode_switching);
            if(isSwStatus(SW_NEXT, click)){
                menu_no++;
                if(menu_no >= 8){
                    menu_no = 0;
                }
            }
            out_indicators(menu_no);

            if(isSwStatus(SW_EXE, click)){
                menu_status = exe_menu;
            }

        } else {
            set_Raspberry_Pi_LED_mode(led_mode_on);
            exe_select_func(menu_no);
        }
    }
}

void init_driver(void)
{
    init_indicators();
    init_TB6612FNG();
    init_sw();
    init_adc();
    init_lineSensor();
    init_cycle();
}

/***********************************************************************************************************************
 * Function Name: menu_no_0
 * Description  : EXEスイッチをクリックすると走行開始
 * Arguments    : none
 * Return Value : none
 ***********************************************************************************************************************/
void menu_no_0(void)
{
    if(isSwStatus(SW_EXE, click)){
        if(get_control_status() == stop){
            set_control_status(switching_to_run);
        } else {
            set_control_status(stop);
        }
    }
}

/***********************************************************************************************************************
 * Function Name: menu_no_1
 * Description  : センサー状態確認処理
 *                シリアルモニターでラインセンサーの状態が確認できる
 * Arguments    : none
 * Return Value : none
 ***********************************************************************************************************************/
void menu_no_1(void)
{
    printf("sensor = %d\n", get_line_center_deff());
    sleep_ms(500);
}

/***********************************************************************************************************************
 * Function Name: menu_no_2
 * Description  : バッテリー値状態確認処理
 * Arguments    : none
 * Return Value : none
 ***********************************************************************************************************************/
void menu_no_2(void)
{
    printf("sensor = %d\n", get_adc_value(3));
    sleep_ms(500);
}

/***********************************************************************************************************************
 * Function Name: menu_no_3
 * Description  : センサー状態確認処理
 *                シリアルモニターでラインセンサーの状態が確認できる
 *                0:黒
 *                1:白
 * Arguments    : none
 * Return Value : none
 ***********************************************************************************************************************/
void menu_no_3(void)
{
    bool *line_sensor_value;
    uint8_t line_sensor_index;

    line_sensor_value = get_line_sensor_row_value_pointer();

    for(line_sensor_index = 0; line_sensor_index < 4; line_sensor_index++){
        if(*line_sensor_value == true){
            printf("1");
        } else {
            printf("0");
        }
        line_sensor_value++;
    }
    printf("\n");
}

/***********************************************************************************************************************
 * Function Name: menu_no_4
 * Description  : モーター接続確認処理
 *                スイッチを押した側のモーターが前進する
 * Arguments    : none
 * Return Value : none
 ***********************************************************************************************************************/
void menu_no_4(void)
{
    if(isSwStatus(SW_EXE, push)){
        set_speed_dc_motor(A, 500);
    } else {
        set_speed_dc_motor(A, 0);
    }

    if(isSwStatus(SW_NEXT, push)){
        set_speed_dc_motor(B, 500);
    } else {
        set_speed_dc_motor(B, 0);
    }
}

void menu_no_5(void)
{
    if(isSwStatus(SW_EXE, click)){

    }
}

void menu_no_6(void)
{
    if(isSwStatus(SW_EXE, click)){

    }
}

void menu_no_7(void)
{
    if(isSwStatus(SW_EXE, click)){

    }
}

void exe_select_func(uint8_t select_func)
{
    (void)(*ptr_f[select_func])();
}
