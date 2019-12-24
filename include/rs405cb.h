/*rs405cb(サーボモータ）プログラム
 * ・関数説明
		void connect(const int Pin,const int BaudRate):モータと接続（pin：RS405通信送受信切り替えポート　BaudRate:モータ通信速度）
		void SendPacket_Short(const int Id,const int Flags,const int Addres,const int Length,const int Count,const int Data):ショートパケット通信の実行(各変数はRS405cbのデータシート参照）
		void SendPacket_Long(const int Addres,const int Length,const int *Data):ロングパケット通信の実行(各変数はRS405cbのデータシート参照）
		int ReturnPacket(const int Id,const int Adress,const int Length):リターンパケットの取得をする。(各変数はRS405cbのデータシート参照）
		void Close():モータとの接続を切断する
		int RightFlush(const int Id):IDに該当するモータ内部メモリをリフレッシュする
		void Reboot(const int Id):IDに該当するモータを再起動する
		int SetBaudRate(const int Id,const int baurate):
		
		・以下関数は、データシートに該当するメモリーマップ値を設定する 
			void SetId(const int Id,const int Id2):
			void SetReverse(const int Id,const int Enable):
			void SetReturnDelay(const int Id,const int Time):
			void SetCWLimit(const int Id,const int Angle):
			void SetCCWLimit(const int Id,const int Angle):
			void SetDamper(const int Id,const int Amount):
			void SetTorqueSilence(const int Id,const int Mode):
			void SetWarmUpTime(const int Id,const int Time):
			void SetCWComplianceMargin(const int Id,const int Angle):
			void SetCCWComplianceMargin(const int Id,const int Angle):
			void SetCWComplianceSlope(const int Id,const int Angle):
			void SetCCWComplianceSlope(const int Id,const int Angle):
			void SetPunch(const int Id,const int Perser):
			void SetPosition(const int Id,const int Angle):
			void SetTime(const int Id,const int Time):
			void SetMaxTorque(const int Id,const int Parser):
			void SetTorqueEnable(const int Id,const int Enable):
		
		・以下関数は、データシートに該当するメモリーマップ値を取得する 
			int GetModelNumber(const int Id):
			int GetFirmwareVersion(const int Id):
			int GetId(const int Id):
			int GetReverse(const int Id):
			int GetBaudRate(const int Id):
			int GetReturnDelay(const int Id):
			int GetCWLimit(const int Id):
			int GetCCWLimit(const int Id):
			int GetTempLimit(const int Id):
			int GetDamper(const int Id):
			int GetTorqueSilence(const int Id):
			int GetWarmUpTime(const int Id):
			int GetCWComplianceMargin(const int Id):
			int GetCCWCompianceMargin(const int Id):
			int GetCWComplianceSlope(const int Id):
			int GetCCWComplianceSlope(const int Id):
			int GetPunch(const int Id):
			int GetGoalPosition(const int Id):
			int GetGoalTime(const int Id):
			int GetMaxTorque(const int Id):
			int GetTorqueEnable(const int Id):
			int GetPosition(const int Id):
			int GetTime(const int Id):
			int GetSpeed(const int Id):
			int GetCurrent(const int Id):
			int GetTemp(const int Id):
			int GetVolts(const int Id):
 */

#ifndef RS405CBH
#define RS405CBH

#include<stdio.h>
#include<stdlib.h>
#include<wiringPi.h>
#include<wiringSerial.h>
#include<unistd.h>
#include<string.h>

const int STARTBIT = 1;
const int PARITYBIT = 0;
const int DATABIT = 8;
const int STOPBIT = 1;
const float RETURNWAIT = 0.01;

//const int BAUDRATE = 460800; // 6leg robot
const int BAUDRATE = 115200;	// default
const int ENABLEPIN = 4; // pin4 is for PURPLE, pin27? is for RED

class rs405cb_c
{
	int a;
public:
	int fd;
	int pin;
	int baudrate;
	int Id[18];
	int angle[18];
	int time[18];
	int torque[18];
	
	rs405cb_c();
	void connect(const int Pin,const int BaudRate);
	void SendPacket_Short(const int Id,const int Flags,const int Addres,const int Length,const int Count,const int Data);
	void SendPacket_Long(const int Addres,const int Length,const int *Data);
	int ReturnPacket(const int Id,const int Adress,const int Length);
	void Close();
	int RightFlush(const int Id);
	void Reboot(const int Id);
	int SetBaudRate(const int Id,const int baurate);
	void SetId(const int Id,const int Id2);
	void SetReverse(const int Id,const int Enable);
	void SetReturnDelay(const int Id,const int Time);
	void SetCWLimit(const int Id,const int Angle);
	void SetCCWLimit(const int Id,const int Angle);
	void SetDamper(const int Id,const int Amount);
	void SetTorqueSilence(const int Id,const int Mode);
	void SetWarmUpTime(const int Id,const int Time);
	void SetCWComplianceMargin(const int Id,const int Angle);
	void SetCCWComplianceMargin(const int Id,const int Angle);
	void SetCWComplianceSlope(const int Id,const int Angle);
	void SetCCWComplianceSlope(const int Id,const int Angle);
	void SetPunch(const int Id,const int Perser);
	void SetPosition(const int Id,const int Angle);
	void SetTime(const int Id,const int Time);
	void SetMaxTorque(const int Id,const int Parser);
	void SetTorqueEnable(const int Id,const int Enable);
	int GetModelNumber(const int Id);
	int GetFirmwareVersion(const int Id);
	int GetId(const int Id);
	int GetReverse(const int Id);
	int GetBaudRate(const int Id);
	int GetReturnDelay(const int Id);
	int GetCWLimit(const int Id);
	int GetCCWLimit(const int Id);
	int GetTempLimit(const int Id);
	int GetDamper(const int Id);
	int GetTorqueSilence(const int Id);
	int GetWarmUpTime(const int Id);
	int GetCWComplianceMargin(const int Id);
	int GetCCWCompianceMargin(const int Id);
	int GetCWComplianceSlope(const int Id);
	int GetCCWComplianceSlope(const int Id);
	int GetPunch(const int Id);
	int GetGoalPosition(const int Id);
	int GetGoalTime(const int Id);
	int GetMaxTorque(const int Id);
	int GetTorqueEnable(const int Id);
	int GetPosition(const int Id);
	int GetTime(const int Id);
	int GetSpeed(const int Id);
	int GetCurrent(const int Id);
	int GetTemp(const int Id);
	int GetVolts(const int Id);
};

#endif // RS405CBH
