/*外部入力スイッチ状態取得プログラム
 *	・使用法
 * 		任意のスイッチに該当する関数を呼び出したときの戻り値から判断（0:押されている　1:押されていない)
 * 			get_T_SW1_enable() -> タクトスイッチ1
 * 			get_T_SW2_enable() -> タクトスイッチ2
 * 			get_T_SW3_enable() -> タクトスイッチ3
 * 			get_T_SW4_enable() -> タクトスイッチ4
 * 			get_S_SW1_enable() -> スライドスイッチ1
 * 			get_S_SW2_enable() -> スライドスイッチ2
 * 			get_S_SW3_enable() -> スライドスイッチ3
 */

#ifndef IOINPUTH
#define IOINPUTH

#include <stdio.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>

#define T_SW1_PIN 22 // tact switch
#define T_SW2_PIN 25
#define T_SW3_PIN 5
#define T_SW4_PIN 12

#define S_SW1_PIN 26 // slide switch
#define S_SW2_PIN 13
#define S_SW3_PIN 19

class io_input_c
{
private:
public:
	io_input_c()
	{
		if (wiringPiSetupGpio() == -1) printf("Connect Error\n");
  
		pinMode(T_SW1_PIN, INPUT);
		pinMode(T_SW2_PIN, INPUT);
		pinMode(T_SW3_PIN, INPUT);
		pinMode(T_SW4_PIN, INPUT);
		pinMode(S_SW1_PIN, INPUT);
		pinMode(S_SW2_PIN, INPUT);
		pinMode(S_SW3_PIN, INPUT);
		
	};
	int get_T_SW1_enable();
	int get_T_SW2_enable();
	int get_T_SW3_enable();
	int get_T_SW4_enable();
	int get_S_SW1_enable();
	int get_S_SW2_enable();
	int get_S_SW3_enable();
};

#endif // IOINPUTH
