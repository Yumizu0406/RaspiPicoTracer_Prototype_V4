/***********************************************************************************************************************
* ファイル名 : indicators.c
* 説明 : 表示モジュール
*        Raspberry Pi Picoボード上のLEDと機体上4つのLEDを制御する
***********************************************************************************************************************/

/***********************************************************************************************************************
include
***********************************************************************************************************************/
#include "indicators.h"
#include "hardware/gpio.h"
#include <stdint.h>

/***********************************************************************************************************************
define and const
***********************************************************************************************************************/
#define SWITCHING_TIME  500u
const uint LED_PIN = PICO_DEFAULT_LED_PIN;

const uint DISP_LED_1_BIT_PIN = 8;
const uint DISP_LED_2_BIT_PIN = 9;
const uint DISP_LED_3_BIT_PIN = 10;
const uint DISP_LED_4_BIT_PIN = 11;

/***********************************************************************************************************************
global
***********************************************************************************************************************/
static uint8_t now_led_disp_value;
static uint16_t led_switching_timer;
static led_mode_t Raspberry_Pi_LED_mode;

/***********************************************************************************************************************
prototype
***********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name: init_indicators
 * Description  : 表示初期化処理
 * Arguments    : none
 * Return Value : none
 ***********************************************************************************************************************/
void init_indicators(void)
{
    Raspberry_Pi_LED_mode = led_mode_on;
    led_switching_timer = 0;
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    now_led_disp_value = 8;
    gpio_init(DISP_LED_1_BIT_PIN);
    gpio_set_dir(DISP_LED_1_BIT_PIN, GPIO_OUT);
    gpio_init(DISP_LED_2_BIT_PIN);
    gpio_set_dir(DISP_LED_2_BIT_PIN, GPIO_OUT);
    gpio_init(DISP_LED_3_BIT_PIN);
    gpio_set_dir(DISP_LED_3_BIT_PIN, GPIO_OUT);
    gpio_init(DISP_LED_4_BIT_PIN);
    gpio_set_dir(DISP_LED_4_BIT_PIN, GPIO_OUT);
}

/***********************************************************************************************************************
 * Function Name: out_indicators
 * Description  : 表示出力処理
 *                機体上4つのLEDに値を2進数で出力する
 * Arguments    : value
 * Return Value : none
 ***********************************************************************************************************************/
void out_indicators(uint8_t value)
{
    const uint DISP_LED_ARRAY[4] = {DISP_LED_1_BIT_PIN, DISP_LED_2_BIT_PIN, DISP_LED_3_BIT_PIN, DISP_LED_4_BIT_PIN};
    uint8_t tmp_value;
    uint8_t bit_count;
    tmp_value = value & 0x0f;

    if(now_led_disp_value == value){
        return;
    }
    now_led_disp_value = value;

    for(bit_count = 0; bit_count < 4; bit_count++){
        if((tmp_value & 0x01) != 0x00){
            gpio_put(DISP_LED_ARRAY[bit_count], false);
        } else {
            gpio_put(DISP_LED_ARRAY[bit_count], true);
        }
        tmp_value = tmp_value >> 1;
    }
}

/***********************************************************************************************************************
 * Function Name: set_Raspberry_Pi_LED_mode
 * Description  : Raspberry Pi LEDモード設定処理
 * Arguments    : mode Raspberry Pi LEDモード
 * Return Value : none
 ***********************************************************************************************************************/
void set_Raspberry_Pi_LED_mode(led_mode_t mode)
{
    Raspberry_Pi_LED_mode = mode;
}

/***********************************************************************************************************************
 * Function Name: update_Raspberry_Pi_LED
 * Description  : Raspberry Pi LED 処理更新
 * Arguments    : none
 * Return Value : none
 ***********************************************************************************************************************/
void update_Raspberry_Pi_LED(void)
{
    if(Raspberry_Pi_LED_mode == led_mode_off){
        gpio_put(LED_PIN, 0);
    } else if(Raspberry_Pi_LED_mode == led_mode_on){
        gpio_put(LED_PIN, 1);
    } else if(Raspberry_Pi_LED_mode == led_mode_switching){ 
        if(led_switching_timer == 0){
            if ( gpio_get(LED_PIN) != 0 ){
                gpio_put(LED_PIN, 0);
            }
            else{
                gpio_put(LED_PIN, 1);
            }
            led_switching_timer = SWITCHING_TIME;
        } else {
            led_switching_timer--;
        }
    }
}