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

#ifndef MPU6050H
#define MPU6050H

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

 public:
	mpu6050_c();
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
};

#endif // MPU6050H
