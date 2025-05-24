/***********************************************************************************************************************
* ファイル名 : lineSensor.c
* 説明 : ラインセンサモジュール
*        使用するラインセンサはフォトトランジスタの値をON/OFFで読み取る想定
***********************************************************************************************************************/

/***********************************************************************************************************************
include
***********************************************************************************************************************/
#include "lineSensor.h"
#include "hardware/gpio.h"

/***********************************************************************************************************************
define and const
***********************************************************************************************************************/
const uint LINE_SENSOR_0_PIN = 3;
const uint LINE_SENSOR_1_PIN = 4;
const uint LINE_SENSOR_2_PIN = 5;
const uint LINE_SENSOR_3_PIN = 6;

/***********************************************************************************************************************
global
***********************************************************************************************************************/
int16_t line_center_deff;
bool line_sensor_row_value[4];

/***********************************************************************************************************************
prototype
***********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name: init_lineSensor
 * Description  : ラインセンサ初期化処理
 *                ラインセンサで使用するピンの初期化および処理で利用する変数の初期化
 * Arguments    : none
 * Return Value : none
 ***********************************************************************************************************************/
void init_lineSensor(void)
{
    gpio_init(LINE_SENSOR_0_PIN);
	gpio_set_dir(LINE_SENSOR_0_PIN, GPIO_IN);

    gpio_init(LINE_SENSOR_1_PIN);
	gpio_set_dir(LINE_SENSOR_1_PIN, GPIO_IN);

    gpio_init(LINE_SENSOR_2_PIN);
	gpio_set_dir(LINE_SENSOR_2_PIN, GPIO_IN);

    gpio_init(LINE_SENSOR_3_PIN);
	gpio_set_dir(LINE_SENSOR_3_PIN, GPIO_IN);

    line_center_deff = 0;
    line_sensor_row_value[0] = false;
    line_sensor_row_value[1] = false;
    line_sensor_row_value[2] = false;
    line_sensor_row_value[3] = false;
}

/***********************************************************************************************************************
 * Function Name: update_lineSensor
 * Description  : ラインセンサ更新処理
 *                フォトトランジスタ直下が
 *                白色だとPINに印可する電圧がLow
 *                白色だとPINに印可する電圧がHigh
 * Arguments    : none
 * Return Value : none
 ***********************************************************************************************************************/
void update_lineSensor(void)
{
    line_sensor_row_value[0] = gpio_get(LINE_SENSOR_0_PIN);
    line_sensor_row_value[1] = gpio_get(LINE_SENSOR_1_PIN);
    line_sensor_row_value[2] = gpio_get(LINE_SENSOR_2_PIN);
    line_sensor_row_value[3] = gpio_get(LINE_SENSOR_3_PIN);

    //黒黒黒白
    if((line_sensor_row_value[0] == true)
    && (line_sensor_row_value[1] == false)
    && (line_sensor_row_value[2] == false)
    && (line_sensor_row_value[3] == false)){
        line_center_deff = 3;
    }

    //黒黒白白
    if((line_sensor_row_value[0] == true)
    && (line_sensor_row_value[1] == true)
    && (line_sensor_row_value[2] == false)
    && (line_sensor_row_value[3] == false)){
        line_center_deff = 2;
    }

    //黒白白白
    if((line_sensor_row_value[0] == true)
    && (line_sensor_row_value[1] == true)
    && (line_sensor_row_value[2] == true)
    && (line_sensor_row_value[3] == false)){
        line_center_deff = 1;
    }

    //白白白白
    if((line_sensor_row_value[0] == true)
    && (line_sensor_row_value[1] == true)
    && (line_sensor_row_value[2] == true)
    && (line_sensor_row_value[3] == true)){
        line_center_deff = 0;
    }

    //白白白黒
    if((line_sensor_row_value[0] == false)
    && (line_sensor_row_value[1] == true)
    && (line_sensor_row_value[2] == true)
    && (line_sensor_row_value[3] == true)){
        line_center_deff = -1;
    }

    //白白黒黒
    if((line_sensor_row_value[0] == false)
    && (line_sensor_row_value[1] == false)
    && (line_sensor_row_value[2] == true)
    && (line_sensor_row_value[3] == true)){
        line_center_deff = -2;
    }

    //白黒黒黒
    if((line_sensor_row_value[0] == false)
    && (line_sensor_row_value[1] == false)
    && (line_sensor_row_value[2] == false)
    && (line_sensor_row_value[3] == true)){
        line_center_deff = -3;
    }
}

/***********************************************************************************************************************
 * Function Name: get_line_center_deff
 * Description  : ラインセンサによる中心からの差分取得処理
 * Arguments    : none
 * Return Value : none
 ***********************************************************************************************************************/
int16_t get_line_center_deff(void)
{
    return line_center_deff;
}

/***********************************************************************************************************************
 * Function Name: get_line_sensor_row_value_pointer
 * Description  : ラインセンサ値のポインタ取得処理
 * Arguments    : none
 * Return Value : none
 ***********************************************************************************************************************/
bool* get_line_sensor_row_value_pointer(void)
{
    return &line_sensor_row_value[0];
}