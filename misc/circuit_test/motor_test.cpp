#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <wiringPi.h>
#include "./hedder/rs405cb.h"

#define BAUDRATE 460800
#define ENABLEPIN 4
#define MOTOR_ID 1

int main(int argv, char **argc)
{
	rs405cb_c rs405cb;
	
	if (wiringPiSetupGpio() == -1) printf("Connect Error\n");
	
	printf("connect %d\n",MOTOR_ID);
	rs405cb.connect(ENABLEPIN, BAUDRATE);
	printf("init\n");
	rs405cb.SetTime(MOTOR_ID, 100);
	rs405cb.SetTorqueEnable(MOTOR_ID, 1);
	printf("set postion 0.0\n");
	rs405cb.SetPosition(MOTOR_ID,  0.0);
	sleep(1);
	printf("set postion -300.0\n");
	rs405cb.SetPosition(MOTOR_ID,  -300.0);
	sleep(1);
	printf("set postion 0.0\n");
	rs405cb.SetPosition(MOTOR_ID,  0.0);
	sleep(1);
	rs405cb.SetTorqueEnable(MOTOR_ID, 0);
}
