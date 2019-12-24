#include <stdio.h>
#include "./hedder/adc.h"

int main(int argc,char ** argv)
{
	adc_c adc;
	while(true)
	{
		printf("VDD:%f\n",adc.get(VDD));
	}
}
