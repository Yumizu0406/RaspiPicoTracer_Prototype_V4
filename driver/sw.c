/***********************************************************************************************************************
* ファイル名 : sw.c
* 説明 : スイッチモジュール
*        チャタリングキャンセル機能付き
***********************************************************************************************************************/

/***********************************************************************************************************************
include
***********************************************************************************************************************/
#include "sw.h"
#include "hardware/gpio.h"

/***********************************************************************************************************************
define and const
***********************************************************************************************************************/
#define SW_PUSH_TIME_THRESHOLD_MS	70u//この指定時間[ms]連続でスイッチがONであればソフトウェアとしてpushされたと判定する
#define SW_NON_PUSH					0xFFFFu

const uint EXE_SW_PIN	= 14;
const uint NEXT_SW_PIN	= 15;

/***********************************************************************************************************************
global
***********************************************************************************************************************/
static uint16_t sw_timer_ms[SW_NUM];//スイッチ用タイマ　スイッチがONではないときはSW_NON_PUSHの値が入る
static sw_status_t current_sw_info[SW_NUM];//現在スイッチ状態
static sw_status_t old_sw_info[SW_NUM];//前回のスイッチ状態

/***********************************************************************************************************************
prototype
***********************************************************************************************************************/
void init_sw_timer(void);
void count_down_sw_timer(void);

/***********************************************************************************************************************
 * Function Name: init_sw
 * Description  : スイッチ初期化処理
 *                スイッチで使用するピンの初期化および処理で利用する変数の初期化
 * Arguments    : none
 * Return Value : none
 ***********************************************************************************************************************/
void init_sw(void)
{
	uint16_t sw_index;

	gpio_init(NEXT_SW_PIN);
	gpio_set_dir(NEXT_SW_PIN, GPIO_IN);//回路でプルアップされてないので内臓プルアップ利用
	gpio_pull_up(NEXT_SW_PIN);

	gpio_init(EXE_SW_PIN);
	gpio_set_dir(EXE_SW_PIN, GPIO_IN);//回路でプルアップされてないので内臓プルアップ利用
	gpio_pull_up(EXE_SW_PIN);

	//変数初期化
	for(sw_index = 0; sw_index < SW_NUM; sw_index++){
		current_sw_info[sw_index] = non;
		old_sw_info[sw_index] = non;
	}

	//タイマ初期化
	init_sw_timer();
}

/***********************************************************************************************************************
 * Function Name: update_sw
 * Description  : スイッチ状態更新処理
 *                1ms周期で更新されることを想定
 * Arguments    : none
 * Return Value : none
 ***********************************************************************************************************************/
void update_sw(void){
	uint16_t sw_index;
	bool sw_port_status[SW_NUM];

	//現在ポート状態取得　スイッチがONされるとGNDにつながるのでfalseになる
	sw_port_status[SW_NEXT] = gpio_get(NEXT_SW_PIN);
	sw_port_status[SW_EXE] = gpio_get(EXE_SW_PIN);

	for(sw_index = 0; sw_index < SW_NUM; sw_index++){
		if(sw_port_status[sw_index] == false){//スイッチがON
			if(sw_timer_ms[sw_index] == SW_NON_PUSH){
				sw_timer_ms[sw_index] = SW_PUSH_TIME_THRESHOLD_MS;
			}
			if(sw_timer_ms[sw_index] <= 0){//連続で指定時間ONになったらpush判定
				current_sw_info[sw_index] = push;
			}
		} else {
			sw_timer_ms[sw_index] = SW_NON_PUSH;//ON計時中にOFFになったら計時リセット
			if(old_sw_info[sw_index] == push){
				current_sw_info[sw_index] = click;
			} else {
				if(current_sw_info[sw_index] != non){
					current_sw_info[sw_index] = non;
				}
			}
		}
		old_sw_info[sw_index] = current_sw_info[sw_index];
	}

	count_down_sw_timer();
}

/***********************************************************************************************************************
 * Function Name: init_sw_timer
 * Description  : スイッチ用タイマ初期化処理
 *                スイッチで利用するタイマすべてを初期化する
 * Arguments    : none
 * Return Value : none
 ***********************************************************************************************************************/
void init_sw_timer(void){
	uint16_t timer_index;
	for(timer_index = 0; timer_index < SW_NUM; timer_index++){
		sw_timer_ms[timer_index] = SW_NON_PUSH;
	}
}

/***********************************************************************************************************************
 * Function Name: count_down_sw_timer
 * Description  : スイッチ用タイマカウントダウン処理
 *                この関数が呼ばれるごとにスイッチ用タイマが1ずつカウントダウン
 * Arguments    : none
 * Return Value : none
 ***********************************************************************************************************************/
void count_down_sw_timer(void){
	uint16_t timer_index;
	for(timer_index = 0; timer_index < SW_NUM; timer_index++){
		if((0 < sw_timer_ms[timer_index]) && (sw_timer_ms[timer_index] != SW_NON_PUSH)){
			sw_timer_ms[timer_index]--;
		}
	}
}

/***********************************************************************************************************************
 * Function Name: isSwStatus
 * Description  : スイッチ状態確認処理
 *                指定したスイッチが指定状態かを確認する
 * Arguments    : sw_type 確認するスイッチ
 *                status 確認するスイッチ状態
 * Return Value : true  実際の状態が指定の状態と一致
 *                false 実際の状態が指定の状態と不一致
 ***********************************************************************************************************************/
bool isSwStatus(sw_type_t sw_type, sw_status_t status){
	bool res = false;
	if(current_sw_info[sw_type] == status){
		//1msの間に複数回clickを読まないように1回読んだらnonにしておく
		if(current_sw_info[sw_type] == click){
			current_sw_info[sw_type] = non;
		}
		res = true;
	}
	return res;
}
