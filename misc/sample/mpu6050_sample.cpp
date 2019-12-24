// gcc -o mpu6050_sample mpu6050_sample.cpp ../mpu6050.cpp -lwiringPi -lm

#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include "include/mpu6050.h"

int main(int argv, char *argc[])
{
    mpu6050_c mpu6050;

    while(true){
	printf("GYRO ");
	printf(" X:%.2f ", mpu6050.GetGYRO_X());
	printf(" Y:%.2f ", mpu6050.GetGYRO_Y());
	printf(" Z:%.2f ", mpu6050.GetGYRO_Z());
	printf("   ACCEL ");
	printf(" X:%.2f ", mpu6050.GetACCEL_X());
	printf(" Y:%.2f ", mpu6050.GetACCEL_Y());
	printf(" Z:%.2f\n",mpu6050.GetACCEL_Z());
    }
    return 0;
}
