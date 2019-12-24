/*ADコンバータプログラム
 * 使用法
 * ①init()でコンバータSPI通信を初期化する（とりあえずプログラムの最初の方におまじないとして宣言しておけばOK）
 * ②adc_read(int channel)でchannelに該当するAD変換値を取得
 * 　※足立制作の六足歩行型ロボットでは、
 * 		ch1  :レンズ正回転時のシャント抵抗電圧
 * 		ch2  :レンズ正回転時のシャント抵抗電圧
 * 		ch3  :電源電圧
 * 		ch3~6:任意の電圧（ピンソケットに入力した電圧）
 * 	　に配線してある
 */

#pragma once
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
	void init();
	double get(int channel);
};

adc_c::adc_c()
{
	if(wiringPiSetupGpio() == -1)printf("connect Error1\n");
	pinMode(CLK_PIN,OUTPUT);
	digitalWrite(CLK_PIN,1);
	if (wiringPiSPISetup(SPI_CH,CLOCK_SPEED) < 0)
	{
		printf("wiringPiSPISetup failed\n");
		return ;
	}	
}
double adc_c::get(int channel)
{
   	data[0] = 0b00000110 | ((channel&0x04)>>2);
   	data[1] = 0b00000000 | ((channel&0x03)<<6);
   	data[2] = 0;
	
	digitalWrite(CLK_PIN,0);
   	wiringPiSPIDataRW(0, data, sizeof(data));
	digitalWrite(CLK_PIN,1);

   	getdata  = (data[1]<<8) & 0xF00;
   	getdata |= (data[2]&0xFF);
	val = (double)VREF * (double)getdata / (double)RESOLUTION;	
    return val;
}

