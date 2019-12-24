/*ADコンバータプログラム
 * 使用法
 * adc_get(int channel)でchannelに該当するAD変換値を取得
 * 　※各チャンネルは、
 * 		ch0  :レンズ正回転時のシャント抵抗電圧
 * 		ch1  :レンズ正回転時のシャント抵抗電圧
 * 		ch2  :電源電圧
 * 		ch3~6:任意の電圧（ピンソケットに入力した電圧）
 * 	　に配線してある
 */

#ifndef ADCH
#define ADCH

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <string.h>
        
#define CLOCK_SPEED 1000000
#define SPI_CH 0
#define CLK_PIN 24
#define VREF 5.0
#define RESOLUTION 4095.0

enum adc_mode
{
	LENS_H = 0,
	LENS_L = 1,
	VDD = 2,
};

class adc_c
{
	private:
	int getdata;
	double val;
	unsigned char data[3];
	public:
	adc_c();
	double get(int channel);
};

#endif // ADCH
