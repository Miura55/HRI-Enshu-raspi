#pragma once

#include <stdio.h>
#include <unistd.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <math.h>	
#include <stdint.h>

#define PWR_MGMT_H 0x6B
#define RA_GYRO_CONFIG 0x1B
#define RA_ACCEL_CONFIG 0x1C
#define INT_ENABLE 0x38
#define WHO_AM_I 0x75
#define CONFIG 0x1A
#define MPU6050_ID 0x68

#define ACCEL_XOUT_H 0x3B
#define ACCEL_YOUT_H 0x3D
#define ACCEL_ZOUT_H 0x3F
#define TEMP_H 0x41
#define GYRO_XOUT_H 0x43
#define GYRO_YOUT_H 0x45
#define GYRO_ZOUT_H 0x47

enum LPF
{
	DLPF_256 = 0x00, 
	DLPF_188 = 0x01, 
	DLPF_98 = 0x02, 
	DLPF_42 = 0x03, 
	DLPF_20 = 0x04, 
	DLPF_10 = 0x05, 
	DLPF_5 = 0x06
};

class mpu6050_c
{
 private:
	int fd;
//	uint8_t data_H, data_L;
//	short buff;
//	double data;

 public:
	mpu6050_c();
	void connect(int ID);
	void init();
	double GetACCEL_X();
	double GetACCEL_Y();
	double GetACCEL_Z();
	double GetSLOPE_X();
	double GetSLOPE_Y();
	double GetSLOPE_Z();
	double GetGYRO_X();
	double GetGYRO_Y();
	double GetGYRO_Z();
	double GetTEMP();
	double GetData(int address);
	double Slope(double data);
};

mpu6050_c::mpu6050_c()
{
	fd = wiringPiI2CSetup(MPU6050_ID);
	if (fd == -1) printf("Init Error1");
	wiringPiI2CWriteReg8(fd, PWR_MGMT_H, 0x70);
	sleep(1);
	wiringPiI2CWriteReg8(fd, RA_GYRO_CONFIG, 0x00);
	wiringPiI2CWriteReg8(fd, RA_ACCEL_CONFIG, 0x00);
	wiringPiI2CWriteReg8(fd, PWR_MGMT_H, 0x00);
}

double mpu6050_c::GetACCEL_X()
{	
        uint8_t data_H, data_L;
	double data;

	data_H = wiringPiI2CReadReg8(fd, ACCEL_XOUT_H);
	data_L = wiringPiI2CReadReg8(fd, ACCEL_XOUT_H+1);
	data = (data_H<<8) + data_L;
	if(data >= 0x8000) data = -((65535 - data) + 1);
	data = (double)(data/16384.0);

	return data;
}

double mpu6050_c::GetACCEL_Y()
{
	uint8_t data_H, data_L;
	double data;

	data_H = wiringPiI2CReadReg8(fd, ACCEL_YOUT_H);
	data_L = wiringPiI2CReadReg8(fd, ACCEL_YOUT_H+1);
	data = (data_H<<8) + data_L;
	if(data >= 0x8000) data = -((65535 - data) + 1);
	data = (double)(data/16384.0);

	return data;
}

double mpu6050_c::GetACCEL_Z()
{
	uint8_t data_H, data_L;
	double data;

	data_H = wiringPiI2CReadReg8(fd, ACCEL_ZOUT_H);
	data_L = wiringPiI2CReadReg8(fd, ACCEL_ZOUT_H+1);
	data = (data_H<<8) + data_L;
	if(data >= 0x8000) data = -((65535 - data) + 1);
	data = (double)(data/16384.0);

	return data;
}

double mpu6050_c::GetSLOPE_X()
{
	uint8_t data_H, data_L;
	double data;

	data_H = wiringPiI2CReadReg8(fd, ACCEL_XOUT_H);
	data_L = wiringPiI2CReadReg8(fd, ACCEL_XOUT_H+1);
	data = (data_H<<8) + data_L;
	if(data >= 0x8000) data = -((65535 - data) + 1);
	data = (double)(data/16384.0);

	if(data > 1) data = 1;
	else if(data < -1) data = -1;
	data = asin(data)*(180.0/M_PI);

	return data;
}

double mpu6050_c::GetSLOPE_Y()
{
        uint8_t data_H, data_L;
	double data;

	data_H = wiringPiI2CReadReg8(fd, ACCEL_YOUT_H);
	data_L = wiringPiI2CReadReg8(fd, ACCEL_YOUT_H+1);
	data = (data_H<<8) + data_L;
	if(data >= 0x8000) data = -((65535 - data) + 1);
	data = (double)(data/16384.0);
	
	if(data > 1) data = 1;
	else if(data < -1) data = -1;
	data = asin(data)*(180.0/M_PI);

	return data;
}

double mpu6050_c::GetSLOPE_Z()
{
        uint8_t data_H, data_L;
	double data;

	data_H = wiringPiI2CReadReg8(fd, ACCEL_ZOUT_H);
	data_L = wiringPiI2CReadReg8(fd, ACCEL_ZOUT_H+1);
	data = (data_H<<8) + data_L;
	if(data >= 0x8000) data = -((65535 - data) + 1);
	data = (double)(data/16384.0);

	if(data > 1) data = 1;
	else if(data < -1) data = -1;
	data = asin(data)*(180.0/M_PI);

	return data;
}

double mpu6050_c::GetGYRO_X()
{
        uint8_t data_H, data_L;
	double data;
	short buff;

	data_H = wiringPiI2CReadReg8(fd, GYRO_XOUT_H);
	data_L = wiringPiI2CReadReg8(fd, GYRO_XOUT_H+1);
	buff = (data_H<<8) | data_L;
	if(buff >= 0x8000) buff = -((65535 - data) + 1);
	data = (double)(buff/131.0);

	return data;
}

double mpu6050_c::GetGYRO_Y()
{	
        uint8_t data_H, data_L;
	double data;
	short buff;

	data_H = wiringPiI2CReadReg8(fd, GYRO_YOUT_H);
	data_L = wiringPiI2CReadReg8(fd, GYRO_YOUT_H+1);
	buff = ((int16_t)data_H<<8) | data_L;
	if (buff >= 0x8000) buff = -((65535 - buff) + 1);
	data = (double)(buff/131.0);

	return data;
}

double mpu6050_c::GetGYRO_Z()
{
        uint8_t data_H, data_L;
	double data;
	short buff;

	data_H = wiringPiI2CReadReg8(fd, GYRO_ZOUT_H);
	data_L = wiringPiI2CReadReg8(fd, GYRO_ZOUT_H+1);
	data = (data_H<<8) + data_L;
	if (data >= 0x8000) data = -((65535 - data) + 1);
	data = (double)(data/131.0);

	return data;
}

double mpu6050_c::GetTEMP()
{
        uint8_t data_H, data_L;
	double data;

	data_H = wiringPiI2CReadReg8(fd, TEMP_H);
	data_L = wiringPiI2CReadReg8(fd, TEMP_H+1);
	data = (data_H<<8) + data_L;
	if (data >= 0x8000) data = -((65535 - data) + 1);
	data = data/340 + 37.53;

	return data;
}

double mpu6050_c::GetData(int address)
{
    int data_H, data_L, data;
	
    data_H = wiringPiI2CReadReg8(fd, address);
    data_L = wiringPiI2CReadReg8(fd, address + 1);
    data = (data_H << 8) + data_L;	
    if (0x8000 <= data) data = (65535 - data + 1) * -1;

    return data;
}

double mpu6050_c::Slope(double data)
{
    if (1.0 < data) data = 1.0;
    else if (data < -1.0) data = -1.0;
    return asin(data) * (180.0 / M_PI);
}

