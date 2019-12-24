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

#include "include/rs405cb.h"

rs405cb_c::rs405cb_c(){
	fd = -1;
	pin = 2;
	baudrate = 9600;
	for(a=0;a<18;a++)
	{
		Id[a] = a+1;
		angle[a] = 0;
		time[a] = 10;
		torque[a] = 0;
	}
}	
void rs405cb_c::connect(const int Pin,const int BaudRate){
	baudrate = BaudRate;
	pin=Pin;
//	fd=serialOpen("/dev/ttyAMA0",BaudRate);  // for Raspbian !!!
	fd=serialOpen("/dev/ttyS0",  BaudRate);  // for Ubuntu !!!
	if(fd == -1){printf("connect Error1\n"); return;}
	pinMode(Pin,OUTPUT);
	serialFlush(fd);
	digitalWrite(pin,0);
}
void rs405cb_c::SendPacket_Short(const int Id,const int Flags,const int Addres,const int Length,const int Count,const int Data){
	int sum,a;
	double sendtime = (1.0/(double)baudrate)*((double)STARTBIT+(double)PARITYBIT+(double)STOPBIT+(double)DATABIT)*(8.0+(double)Length);

	if(fd == -1){printf("SendPacket_Short Error1\n");return;}

	sum = Id ^ Flags ^ Addres ^ Length ^ 0x01;
	for(a=0;a<Length;a++)sum ^= (Data & (0xFF<<(8*a)))>>(8*a);

	digitalWrite(pin,1);

	serialPutchar(fd,0xFA);
	serialPutchar(fd,0xAF);
	serialPutchar(fd,Id);	
	serialPutchar(fd,Flags);
	serialPutchar(fd,Addres);
	serialPutchar(fd,Length);
	serialPutchar(fd,Count);
	for(a=0;a<Length;a++) serialPutchar(fd,(Data & (0xFF<<(8*a)))>>(8*a));
	serialPutchar(fd,sum);
	usleep(sendtime*1000.0*1000.0);

	digitalWrite(pin,0);
}
void rs405cb_c::SendPacket_Long(const int Addres,const int Length,const int *Data){
	int sum=0,a,b;
	int Count = 18;
	double sendtime = (1.0/(double)baudrate)*((double)STARTBIT+(double)PARITYBIT+(double)STOPBIT+(double)DATABIT)*(8.0+(double)(Length+1)*(double)Count);

	if(fd == -1){printf("SendPacket_Long Error1\n");return;}
	sum = 0x00 ^ Addres ^ (Length+1) ^ Count;
	for(a=0;a<Count;a++)
	{
		sum ^= Id[a];
		for(b=0;b<Length;b++) sum ^= (Data[a] & (0xFF<<(8*b)))>>(8*b);
	}
		digitalWrite(pin,1);

	serialPutchar(fd,0xFA);
	serialPutchar(fd,0xAF);
	serialPutchar(fd,0x00);
	serialPutchar(fd,0x00);
	serialPutchar(fd,Addres);
	serialPutchar(fd,Length+1);
	serialPutchar(fd,Count);
	for(a=0;a<Count;a++)
	{
		serialPutchar(fd,Id[a]);
		for(b=0;b<Length;b++) serialPutchar(fd,(Data[a] & (0xFF<<(8*b)))>>(8*b));
	}

	serialPutchar(fd,sum);
	usleep(sendtime*1000.0*1000.0);
	digitalWrite(pin,0);
}
int rs405cb_c::ReturnPacket(const int Id,const int Adress,const int Length){
	int sum,ReturnPacket[8+Length],data=0;
	double sendtime = (1.0/(double)baudrate)*((double)STARTBIT+(double)PARITYBIT+(double)STOPBIT+(double)DATABIT)*8.0;
	double recivetime = (1.0/(double)baudrate)*((double)STARTBIT+(double)PARITYBIT+(double)STOPBIT+(double)DATABIT)*(8.0+(double)Length)+RETURNWAIT;
	int a=0;

	memset(ReturnPacket,0x00,sizeof(ReturnPacket));
	sum = Id ^ 0x0F ^ Adress ^ Length ^ 0x00;

	digitalWrite(pin,1);
	serialPutchar(fd,0xFA);
	serialPutchar(fd,0xAF);
	serialPutchar(fd,Id);
	serialPutchar(fd,0x0F);
	serialPutchar(fd,Adress);
	serialPutchar(fd,Length);
	serialPutchar(fd,0x00);
	serialPutchar(fd,sum);
	usleep(sendtime*1000.0*1000.0);
	digitalWrite(pin,0);

	usleep(recivetime*1000.0*1000.0);
	if(serialDataAvail(fd)==0){return -1;}
	while(serialDataAvail(fd))
	{
		if(serialDataAvail(fd) <= 8+Length){ReturnPacket[a] = serialGetchar(fd);a++;}
		else {serialGetchar(fd);}
	}
	for(a=0;a<Length;a++)data |= ReturnPacket[7+a]<<8*a; 
	sum=0;
	for(a=2;a<7+Length;a++)sum ^= ReturnPacket[a];
	if(sum==ReturnPacket[7+Length]){return data;}
	else {return -1;}
}
void rs405cb_c::Close(){
	serialClose(fd);	
}
int rs405cb_c::RightFlush(const int Id){
	int sum;
	double sendtime = (1.0/(double)baudrate)*((double)STARTBIT+(double)PARITYBIT+(double)STOPBIT+(double)DATABIT)*8.0;
		if(fd == -1){return -1;}

	sum = Id ^ 0x40 ^ 0xFF ^ 0x00 ^ 0x00;
	
	digitalWrite(pin,1);

	serialPutchar(fd,0xFA);
	serialPutchar(fd,0xAF);
	serialPutchar(fd,Id);
	serialPutchar(fd,0x40);
	serialPutchar(fd,0xFF);
	serialPutchar(fd,0x00);
	serialPutchar(fd,0x00);
	serialPutchar(fd,sum);
	usleep(sendtime*1000.0*1000.0);

	digitalWrite(pin,0);
	usleep(1000*1000);
	return 0;
}
void rs405cb_c::Reboot(const int Id){
	int sum;
	double sendtime = (1.0/(double)baudrate)*((double)STARTBIT+(double)PARITYBIT+(double)STOPBIT+(double)DATABIT)*8.0;
		if(fd == -1){printf("SendPacket_Short Error1\n");return;}

	sum = Id ^ 0x20 ^ 0xFF ^ 0x00 ^ 0x00;
	
	digitalWrite(pin,1);

	serialPutchar(fd,0xFA);
	serialPutchar(fd,0xAF);
	serialPutchar(fd,Id);
	serialPutchar(fd,0x20);
	serialPutchar(fd,0xFF);
	serialPutchar(fd,0x00);
	serialPutchar(fd,0x00);
	serialPutchar(fd,sum);
	usleep(sendtime*1000.0*1000.0);

	digitalWrite(pin,0);
	usleep(1000*100);
}
int rs405cb_c::SetBaudRate(const int Id,const int baurate){
	switch(baurate)
	{
		case 9600: SendPacket_Short(Id,0x00,0x06,0x01,0x01,0x00);break;
		case 14400: SendPacket_Short(Id,0x00,0x06,0x01,0x01,0x01);break;
		case 19200: SendPacket_Short(Id,0x00,0x06,0x01,0x01,0x02);break;
		case 28800: SendPacket_Short(Id,0x00,0x06,0x01,0x01,0x03);break;
		case 38400: SendPacket_Short(Id,0x00,0x06,0x01,0x01,0x04);break;
		case 57600: SendPacket_Short(Id,0x00,0x06,0x01,0x01,0x05);break;
		case 76800: SendPacket_Short(Id,0x00,0x06,0x01,0x01,0x06);break;
		case 115200: SendPacket_Short(Id,0x00,0x06,0x01,0x01,0x07);break;
		case 153600: SendPacket_Short(Id,0x00,0x06,0x01,0x01,0x08);break;
		case 230400: SendPacket_Short(Id,0x00,0x06,0x01,0x01,0x09);break;
		case 460800: SendPacket_Short(Id,0x00,0x06,0x01,0x01,0x0A);break;
		default:return -1;break;
	}
}
void rs405cb_c::SetId(const int Id,const int Id2){                   SendPacket_Short(Id,0x00,0x04,0x01,0x01,Id2);}
void rs405cb_c::SetReverse(const int Id,const int Enable){           SendPacket_Short(Id,0x00,0x05,0x01,0x01,Enable);}
void rs405cb_c::SetReturnDelay(const int Id,const int Time){         SendPacket_Short(Id,0x00,0x07,0x01,0x01,Time);}
void rs405cb_c::SetCWLimit(const int Id,const int Angle){            SendPacket_Short(Id,0x00,0x08,0x02,0x01,Angle);}
void rs405cb_c::SetCCWLimit(const int Id,const int Angle){           SendPacket_Short(Id,0x00,0x0A,0x02,0x01,Angle);}
void rs405cb_c::SetDamper(const int Id,const int Amount){            SendPacket_Short(Id,0x00,0x14,0x01,0x01,Amount);}
void rs405cb_c::SetTorqueSilence(const int Id,const int Mode){       SendPacket_Short(Id,0x00,0x16,0x01,0x01,Mode);}
void rs405cb_c::SetWarmUpTime(const int Id,const int Time){          SendPacket_Short(Id,0x00,0x17,0x01,0x01,Time);}
void rs405cb_c::SetCWComplianceMargin(const int Id,const int Angle){ SendPacket_Short(Id,0x00,0x18,0x01,0x01,Angle);}
void rs405cb_c::SetCCWComplianceMargin(const int Id,const int Angle){SendPacket_Short(Id,0x00,0x19,0x01,0x01,Angle);}
void rs405cb_c::SetCWComplianceSlope(const int Id,const int Angle){  SendPacket_Short(Id,0x00,0x1A,0x01,0x01,Angle);}
void rs405cb_c::SetCCWComplianceSlope(const int Id,const int Angle){ SendPacket_Short(Id,0x00,0x1B,0x01,0x01,Angle);}
void rs405cb_c::SetPunch(const int Id,const int Perser){             SendPacket_Short(Id,0x00,0x1C,0x02,0x01,Perser);}
void rs405cb_c::SetPosition(const int Id,const int Angle){           SendPacket_Short(Id,0x00,0x1E,0x02,0x01,Angle);}
void rs405cb_c::SetTime(const int Id,const int Time){                SendPacket_Short(Id,0x00,0x20,0x02,0x01,Time);}
void rs405cb_c::SetMaxTorque(const int Id,const int Parser){         SendPacket_Short(Id,0x00,0x23,0x01,0x01,Parser);}
void rs405cb_c::SetTorqueEnable(const int Id,const int Enable){      SendPacket_Short(Id,0x00,0x24,0x01,0x01,Enable);}
int rs405cb_c::GetModelNumber(const int Id)		{return ReturnPacket(Id,0x00,2);}
int rs405cb_c::GetFirmwareVersion(const int Id)	{return ReturnPacket(Id,0x02,1);}
int rs405cb_c::GetId(const int Id)					{return ReturnPacket(Id,0x04,1);}
int rs405cb_c::GetReverse(const int Id)			{return ReturnPacket(Id,0x05,1);}
int rs405cb_c::GetBaudRate(const int Id){
	switch(ReturnPacket(Id,0x06,1))
	{
		case 0x00:return 9600;break;
		case 0x01:return 14400;break;
		case 0x02:return 19200;break;
		case 0x03:return 28800;break;
		case 0x04:return 38400;break;
		case 0x05:return 57600;break;
		case 0x06:return 76800;break;
		case 0x07:return 115200;break;
		case 0x08:return 153600;break;
		case 0x09:return 230400;break;
		case 0x0A:return 460800;break;
		default :return -1;break;
	}
}
int rs405cb_c::GetReturnDelay(const int Id)		{return ReturnPacket(Id,0x07,1);}
int rs405cb_c::GetCWLimit(const int Id)			{return ReturnPacket(Id,0x08,2);}
int rs405cb_c::GetCCWLimit(const int Id)			{return ReturnPacket(Id,0x0A,2);}
int rs405cb_c::GetTempLimit(const int Id)			{return ReturnPacket(Id,0x0E,2);}
int rs405cb_c::GetDamper(const int Id)				{return ReturnPacket(Id,0x14,1);}
int rs405cb_c::GetTorqueSilence(const int Id)		{return ReturnPacket(Id,0x16,1);}
int rs405cb_c::GetWarmUpTime(const int Id)			{return ReturnPacket(Id,0x17,1);}
int rs405cb_c::GetCWComplianceMargin(const int Id)	{return ReturnPacket(Id,0x18,1);}
int rs405cb_c::GetCCWCompianceMargin(const int Id)	{return ReturnPacket(Id,0x19,1);}
int rs405cb_c::GetCWComplianceSlope(const int Id)	{return ReturnPacket(Id,0x1A,1);}
int rs405cb_c::GetCCWComplianceSlope(const int Id)	{return ReturnPacket(Id,0x1B,1);}
int rs405cb_c::GetPunch(const int Id)				{return ReturnPacket(Id,0x1C,2);}
int rs405cb_c::GetGoalPosition(const int Id)		{return ReturnPacket(Id,0x1E,2);}
int rs405cb_c::GetGoalTime(const int Id)			{return ReturnPacket(Id,0x20,2);}
int rs405cb_c::GetMaxTorque(const int Id)			{return ReturnPacket(Id,0x23,1);}
int rs405cb_c::GetTorqueEnable(const int Id)		{return ReturnPacket(Id,0x24,1);}
int rs405cb_c::GetPosition(const int Id)			{return ReturnPacket(Id,0x2A,2);}
int rs405cb_c::GetTime(const int Id)				{return ReturnPacket(Id,0x2C,2);}
int rs405cb_c::GetSpeed(const int Id)				{return ReturnPacket(Id,0x2E,2);}
int rs405cb_c::GetCurrent(const int Id)			{return ReturnPacket(Id,0x30,2);}
int rs405cb_c::GetTemp(const int Id)				{return ReturnPacket(Id,0x32,2);}
int rs405cb_c::GetVolts(const int Id)				{return ReturnPacket(Id,0x34,2);}
