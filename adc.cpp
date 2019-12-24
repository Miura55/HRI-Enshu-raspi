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

#include "include/adc.h"

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

