/*mpu6050(9軸角加速度センサ）プログラム
 * ・関数説明
 * 		GetACCEL_X():X軸の加速度を取得
 * 		GetACCEL_Y():Y軸の加速度を取得
 * 		GetACCEL_Z():Z軸の加速度を取得
 * 		GetSLOPE_X():X軸の角度を取得
 * 		GetSLOPE_Y():Y軸の角度を取得
 * 		GetSLOPE_Z():Z軸の角度を取得
 * 		GetGYRO_X():X軸の角速度を取得
 * 		GetGYRO_Y():X軸の角速度を取得
 * 		GetGYRO_Z():X軸の角速度を取得
 * 		GetTEMP():内部温度センサの温度を取得
 * 		GetData(int addres):addresに沿ったデータを取得（addresはmpu6050のデータシート参照）
 */

#include "include/mpu6050.h"

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

