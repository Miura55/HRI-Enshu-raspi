// gcc -o adc_sample adc_sample.cpp ../adc.cpp -lwiringPi

#include <stdio.h>
#include "include/adc.h"

int main(int argc, char *argv[])
{
	adc_c adc;

	while(true)
	{
		printf("VDD:%f\n", adc.get(VDD)*4.0);
	}
}
