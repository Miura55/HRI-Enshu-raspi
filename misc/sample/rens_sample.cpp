// gcc -o rens_sample rens_sample.cpp ../io_input.cpp -lwiringPi

#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <wiringPi.h>
#include "include/io_input.h"

#define MAIN_ENA 6
#define IRIS_ENA 16
#define ZOOM_ENA 20
#define FOCUS_ENA 21

int main(int argv, char *argc[])
{
	int i;
	io_input_c io_input;
	
	if (wiringPiSetupGpio() == -1) printf("Connect Error\n");
  
	pinMode(MAIN_ENA, OUTPUT);
	pinMode(IRIS_ENA, OUTPUT);
	pinMode(ZOOM_ENA, OUTPUT);
	pinMode(FOCUS_ENA, OUTPUT);

	digitalWrite(MAIN_ENA,0);
	digitalWrite(IRIS_ENA,0);
	digitalWrite(ZOOM_ENA,0);
	digitalWrite(FOCUS_ENA,0);
	
	while(1){
		//MAIN
		digitalWrite(MAIN_ENA,io_input.get_D_SW4_enable());
		
		//IRIS
		if(io_input.get_D_SW1_enable() == 0)digitalWrite(IRIS_ENA,1);
		else digitalWrite(IRIS_ENA,0);
		
		//ZOOM
		if(io_input.get_D_SW2_enable() == 0)digitalWrite(ZOOM_ENA,1);
		else digitalWrite(ZOOM_ENA,0);
		
		//FOCUS
		if(io_input.get_D_SW3_enable() == 0)digitalWrite(FOCUS_ENA,1);
		else digitalWrite(FOCUS_ENA,0);
				
		printf("IRIS:%d ZOOM:%d FOCUS:%d OUTPUT:%d\n",
			io_input.get_D_SW1_enable(),
			io_input.get_D_SW2_enable(),
			io_input.get_D_SW3_enable(),
			io_input.get_D_SW4_enable()
		);
		
	}
}
