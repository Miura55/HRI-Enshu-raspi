// Raspbian
// cc -o mpudemo mpudemo.cpp mpu6050.cpp -lwiringPi -lm
// gcc -o mpudemo mpudemo.cpp mpu6050.cpp -lwiringPi -lm

// Ubuntu
// cc -o mpudemo mpudemo.cpp mpu6050.cpp -lwiringPi -lpthread -lcrypt -lrt -lm
// gcc -o mpudemo mpudemo.cpp mpu6050.cpp -lwiringPi -lpthread -lcrypt -lrt -lm

// sudo ./mpudemo

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <wiringPi.h>
#include "./hedder/mpu6050.h"

#define DATANUM 100

#define PXGAIN  1.0           // Plus x gain
#define MXGAIN  (90.0 / 74.0) // Minus x gain
#define PYGAIN  (90.0 / 78.0) // Plus y gain
#define MYGAIN  (90.0 / 89.0) // Minus y gain
#define PZGAIN  1.0           // Plus z gain
#define MZGAIN  (90.0 / 73.5) // Minus z gain

int main(int argv, char **argc){
    //init
    mpu6050_c mpu6050;

    if (wiringPiSetupGpio() == -1) printf("connect Error1\n");
    //mpu6050.connect(MPU6050_ID);
    //mpu6050.init();

    int    i, idx;
    double a_x[DATANUM], a_y[DATANUM], a_z[DATANUM];
    double g_x, g_y, g_z;
    double s_x, s_y, s_z; // slope
    double temp;

    idx = 0;
	
    while (1) {
        a_x[idx] = mpu6050.GetData(ACCEL_XOUT_H) / 16384.0;
	a_y[idx] = mpu6050.GetData(ACCEL_YOUT_H) / 16384.0;
	a_z[idx] = mpu6050.GetData(ACCEL_ZOUT_H) / 16384.0;
	g_x = mpu6050.GetData(GYRO_XOUT_H) / 131.0;
	g_y = mpu6050.GetData(GYRO_YOUT_H) / 131.0;
	g_z = mpu6050.GetData(GYRO_ZOUT_H) / 131.0;
	temp = mpu6050.GetData(TEMP_H) / 340 + 36.53;

	s_x = s_y = s_z = 0.0;
	for (i = 0; i < DATANUM; i++) {
	    s_x += mpu6050.Slope(a_x[i]);
	    s_y += mpu6050.Slope(a_y[i]);
	    s_z += mpu6050.Slope(a_z[i]);
	} 
	s_x /= DATANUM; if (s_x < 0.0) s_x *= MXGAIN; else s_x *= PXGAIN;
	s_y /= DATANUM; if (s_y < 0.0) s_y *= MYGAIN; else s_y *= PYGAIN;
	s_z /= DATANUM; if (s_z < 0.0) s_z *= MZGAIN; else s_z *= PZGAIN;

	if (0 == idx % 20) {
	    printf("%5.1f %5.1f %5.1f     %7.1f %7.1f %7.1f     %5.1f\n",
		   s_x, s_y, s_z, g_x, g_y, g_z, temp) ;
	}
	
	idx++;
	if (DATANUM == idx) idx = 0;
    }
    return 0;

  /*
  while(true){
    system("clear");
    printf("GYRO ");
    printf(" X:%.2f ", mpu6050.GetGYRO_X());
    printf(" Y:%.2f ", mpu6050.GetGYRO_Y());
    printf(" Z:%.2f ", mpu6050.GetGYRO_Z());
    printf("   ACCEL ");
    printf(" X:%.2f ", mpu6050.GetACCEL_X());
    printf(" Y:%.2f ", mpu6050.GetACCEL_Y());
    printf(" Z:%.2f\n",mpu6050.GetACCEL_Z());
    sleep(1);
  }
  */
}
