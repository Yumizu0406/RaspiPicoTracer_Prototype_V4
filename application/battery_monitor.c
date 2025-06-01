/***********************************************************************************************************************
* ファイル名 : battery_monitor.c
* 説明 : バッテリーモジュール
***********************************************************************************************************************/

/***********************************************************************************************************************
include
***********************************************************************************************************************/
#include "battery_monitor.h"
#include "../driver/adchange.h"
#include <stdint.h>

/***********************************************************************************************************************
define and const
***********************************************************************************************************************/
#define VATTERY_ADC_BUFF_SIZE	    4u
#define TOPSIDE_RESISTER_VALUE      200000u
#define BOTTOMSIDE_RESISTER_VALUE	100000u
#define RESISTER_VALUE_SUM	(TOPSIDE_RESISTER_VALUE + BOTTOMSIDE_RESISTER_VALUE)
#define AVSS_VOLTAGE                3300u
#define ADC_MAX_VALUE               4095u
/***********************************************************************************************************************
global
***********************************************************************************************************************/
static uint16_t vattery_voltage_buff[VATTERY_ADC_BUFF_SIZE];
static uint16_t battery_voltage_mV;
static uint8_t battery_buff_count;
/***********************************************************************************************************************
prototype
***********************************************************************************************************************/
uint16_t calculate_trimmed_mean_vattery_voltage(void);

/***********************************************************************************************************************
 * Function Name: init_battery
 * Description  : バッテリーモジュール初期化処理
 * Arguments    : none
 * Return Value : none
 ***********************************************************************************************************************/
void init_battery(void)
{
    uint8_t buff_index;

    for(buff_index = 0u; buff_index < VATTERY_ADC_BUFF_SIZE; buff_index++ ){
        vattery_voltage_buff[buff_index] = 0u;
    }

    battery_buff_count = 0u;
}

/***********************************************************************************************************************
 * Function Name: update_battery
 * Description  : バッテリー値更新処理
 * Arguments    : none
 * Return Value : none
 ***********************************************************************************************************************/
void update_battery(void)
{
	float calculate_voltage;
	unsigned short ad_value;
	float ad_voltage;

	ad_value = get_adc_value(3);

	//抵抗器で分圧回路を作ってあるので抵抗値から電源電圧値を計算する
	ad_voltage = ((float)ad_value / (float)ADC_MAX_VALUE) * (float)AVSS_VOLTAGE;
	calculate_voltage = ((float)RESISTER_VALUE_SUM / (float)BOTTOMSIDE_RESISTER_VALUE) * ad_voltage;

    //計算した値はリングバファに記録する
	vattery_voltage_buff[battery_buff_count] = (uint16_t)calculate_voltage;
    battery_buff_count++;
	if(VATTERY_ADC_BUFF_SIZE <= battery_buff_count){
		battery_buff_count = 0;
	}

	battery_voltage_mV = calculate_trimmed_mean_vattery_voltage();
}

/***********************************************************************************************************************
 * Function Name: calculate_trimmed_mean_vattery_voltage
 * Description  : トリム移動平均を計算する
 * Arguments    : none
 * Return Value : none
 ***********************************************************************************************************************/
uint16_t calculate_trimmed_mean_vattery_voltage(void)
{
	uint8_t buff_index;
	uint16_t max_ad_value = 0u;
	uint16_t min_ad_value = 0xFFFFu;
	uint32_t sum_ad_value = 0u;
	uint32_t sum_ad_value_without_max_min;
	uint32_t result_uint32;

	for(buff_index = 0; buff_index < VATTERY_ADC_BUFF_SIZE; buff_index++){
		if(max_ad_value < vattery_voltage_buff[buff_index]){
			max_ad_value = vattery_voltage_buff[buff_index];
		}
		if(vattery_voltage_buff[buff_index] < min_ad_value){
			min_ad_value = vattery_voltage_buff[buff_index];
		}
		sum_ad_value += (uint32_t)vattery_voltage_buff[buff_index];
	}
	sum_ad_value_without_max_min = sum_ad_value - (uint32_t)max_ad_value - (uint32_t)min_ad_value;
	result_uint32 = sum_ad_value_without_max_min / (VATTERY_ADC_BUFF_SIZE - 2u);

	return (uint16_t)result_uint32;
}

/***********************************************************************************************************************
 * Function Name: get_battery_voltage_mV
 * Description  : バッテリーの電圧値を取得する
 * Arguments    : none
 * Return Value : none
 ***********************************************************************************************************************/
uint16_t get_battery_voltage_mV(void)
{
	return battery_voltage_mV;
}