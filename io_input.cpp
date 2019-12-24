/*外部入力スイッチ状態取得プログラム
 *	・使用法
 * 	任意のスイッチに該当する関数を呼び出したときの戻り値から判断
 *      （0:押されている　1:押されていない)
 * 	get_T_SW1_enable() -> タクトスイッチ1
 * 	get_T_SW2_enable() -> タクトスイッチ2
 * 	get_T_SW3_enable() -> タクトスイッチ3
 * 	get_T_SW4_enable() -> タクトスイッチ4
 * 	get_S_SW1_enable() -> スライドスイッチ1
 * 	get_S_SW2_enable() -> スライドスイッチ2
 * 	get_S_SW3_enable() -> スライドスイッチ3
 */

#include "include/io_input.h"

int io_input_c::get_T_SW1_enable(){return digitalRead(T_SW1_PIN);};
int io_input_c::get_T_SW2_enable(){return digitalRead(T_SW2_PIN);};
int io_input_c::get_T_SW3_enable(){return digitalRead(T_SW3_PIN);};
int io_input_c::get_T_SW4_enable(){return digitalRead(T_SW4_PIN);};
int io_input_c::get_S_SW1_enable(){return digitalRead(S_SW1_PIN);};
int io_input_c::get_S_SW2_enable(){return digitalRead(S_SW2_PIN);};
int io_input_c::get_S_SW3_enable(){return digitalRead(S_SW3_PIN);};
