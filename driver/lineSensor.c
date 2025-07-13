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
#define RECOGNIZING_TIME    200

const uint LINE_SENSOR_0_PIN = 3;
const uint LINE_SENSOR_1_PIN = 4;
const uint LINE_SENSOR_2_PIN = 5;
const uint LINE_SENSOR_3_PIN = 6;
const uint SG_MARKER_SENSOR_PIN = 2;
const uint CORNER_MARKER_SENSOR_PIN = 7;

typedef enum{
	control_timer_goal_marker_recognizing,
	control_timer_corner_marker_recognizing,
	control_timer_num
}control_timer_t;

typedef enum{
    undetected = 0,
    detecting,
    judge,
    wait_undetected
}goal_marker_judge_state_t;
/***********************************************************************************************************************
global
***********************************************************************************************************************/
static int16_t line_center_deff;
static bool line_sensor_row_value[4];
static bool start_goal_marker_value;
static bool corner_marker_value;
static goal_marker_judge_state_t goal_marker_judge_state;
static uint16_t control_timer[control_timer_num];
static uint8_t goal_marker_count;
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

    gpio_init(SG_MARKER_SENSOR_PIN);
	gpio_set_dir(SG_MARKER_SENSOR_PIN, GPIO_IN);

    gpio_init(CORNER_MARKER_SENSOR_PIN);
	gpio_set_dir(CORNER_MARKER_SENSOR_PIN, GPIO_IN);

    line_center_deff = 0;
    line_sensor_row_value[0] = false;
    line_sensor_row_value[1] = false;
    line_sensor_row_value[2] = false;
    line_sensor_row_value[3] = false;
    start_goal_marker_value = false;
    corner_marker_value = false;
    goal_marker_judge_state = undetected;
    control_timer[control_timer_goal_marker_recognizing] = 0u;
    control_timer[control_timer_corner_marker_recognizing] = 0u;
    goal_marker_count = 0;
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

/***********************************************************************************************************************
 * Function Name: update_goal_marker_judge_state
 * Description  : ゴールマーカー検出処理
 *                1msごとに呼び出される想定
 * Arguments    : none
 * Return Value : none
 ***********************************************************************************************************************/
 void update_goal_marker_judge_state(void)
{
    //タイマー更新処理
    int8_t control_timer_index;
    for(control_timer_index = 0u; control_timer_index < control_timer_num; control_timer_index++){
        if(0u < control_timer[control_timer_index]){
            control_timer[control_timer_index]--;
        }
    }

    //ゴールマーカーとコーナーマーカーの値取得
    start_goal_marker_value = gpio_get(SG_MARKER_SENSOR_PIN);
    corner_marker_value = gpio_get(CORNER_MARKER_SENSOR_PIN);

    //ゴールマーカー検出処理実施
    if(goal_marker_judge_state == wait_undetected){
        if((start_goal_marker_value == false) && (corner_marker_value == false)){
        	goal_marker_judge_state = undetected;
        }
    }
    if(((goal_marker_judge_state == undetected) || (goal_marker_judge_state == detecting)) &&
        	(start_goal_marker_value == true) && (control_timer[control_timer_goal_marker_recognizing] <= 0u)){
        	control_timer[control_timer_goal_marker_recognizing] = RECOGNIZING_TIME;
        	goal_marker_judge_state = detecting;
    }
    if(((goal_marker_judge_state == undetected) || (goal_marker_judge_state == detecting)) &&
        (corner_marker_value == true) && (control_timer[control_timer_corner_marker_recognizing] <= 0u)){
        control_timer[control_timer_corner_marker_recognizing] = RECOGNIZING_TIME;
        goal_marker_judge_state = detecting;
    }
    if((0 < control_timer[control_timer_goal_marker_recognizing]) && (0u < control_timer[control_timer_corner_marker_recognizing])){
        control_timer[control_timer_goal_marker_recognizing] = 0u;
        control_timer[control_timer_corner_marker_recognizing] = 0u;
        goal_marker_judge_state = wait_undetected;
    }

    if(goal_marker_judge_state == detecting){
        if((control_timer[control_timer_goal_marker_recognizing] <= 0u) && (control_timer[control_timer_corner_marker_recognizing] <= 1u)){
        	control_timer[control_timer_goal_marker_recognizing] = 0u;
        	control_timer[control_timer_corner_marker_recognizing] = 0u;
        	goal_marker_judge_state = wait_undetected;
        } else if((control_timer[control_timer_goal_marker_recognizing] <= 1u) && (control_timer[control_timer_corner_marker_recognizing] == 0u)){
        	goal_marker_count++;
            goal_marker_judge_state = wait_undetected;
        }
    }
}

/***********************************************************************************************************************
 * Function Name: get_goal_marker_count
 * Description  : ゴールマーカー数取得処理
 * Arguments    : none
 * Return Value : none
 ***********************************************************************************************************************/
uint8_t get_goal_marker_count(void)
{
    return goal_marker_count;
}

/***********************************************************************************************************************
 * Function Name: clear_goal_marker_count
 * Description  : ゴールマーカー数クリア処理
 * Arguments    : none
 * Return Value : none
 ***********************************************************************************************************************/
void clear_goal_marker_count(void)
{
    goal_marker_count = 0;
}
