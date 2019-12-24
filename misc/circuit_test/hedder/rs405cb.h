#pragma once 
#include<stdio.h>
#include<stdlib.h>
#include<wiringPi.h>
#include<wiringSerial.h>
#include<unistd.h>
#include<string.h>

#define STARTBIT 1
#define PARITYBIT 0
#define DATABIT 8
#define STOPBIT 1
#define RETURNWAIT 0.01

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
	rs405cb_c()
	{
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
	void connect(const int Pin,const int BaudRate)
	{
		baudrate = BaudRate;
		pin=Pin;
		fd=serialOpen("/dev/ttyAMA0",BaudRate);
		if(fd == -1){printf("connect Error1\n"); return;}
		//if(wiringPiSetupGpio() == -1){printf("connect Error1\n"); return;}
		pinMode(Pin,OUTPUT);
		serialFlush(fd);
		digitalWrite(pin,0);
	}

	void SendPacket_Short(const int Id,const int Flags,const int Addres,const int Length,const int Count,const int Data)
	{
	        int sum,a;
		double sendtime = (1.0/(double)baudrate)*((double)STARTBIT+(double)PARITYBIT+(double)STOPBIT+(double)DATABIT)*(8.0+(double)Length);
	
		//printf("sendtime =%.4f\n",sendtime);
	
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

	void SendPacket_Long(const int Addres,const int Length,const int *Data)
	{
		int sum=0,a,b;
		int Count = 18;
		double sendtime = (1.0/(double)baudrate)*((double)STARTBIT+(double)PARITYBIT+(double)STOPBIT+(double)DATABIT)*(8.0+(double)(Length+1)*(double)Count);

		//wiringPiSetupGpio();
		//pinMode(pin,OUTPUT);
		if(fd == -1){printf("SendPacket_Long Error1\n");return;}
		//printf("pin:%d\n",pin);
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

	int ReturnPacket(const int Id,const int Adress,const int Length)
	{
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
		//printf("%d ",serialDataAvail(fd));
		if(serialDataAvail(fd)==0){return -1;}
		while(serialDataAvail(fd))
		{
			if(serialDataAvail(fd) <= 8+Length){ReturnPacket[a] = serialGetchar(fd);a++;}
			else {serialGetchar(fd);}
		}
		//for(a=0;a<8+Length;a++)printf("%2x ",ReturnPacket[a]);
		for(a=0;a<Length;a++)data |= ReturnPacket[7+a]<<8*a; 
		sum=0;
		for(a=2;a<7+Length;a++)sum ^= ReturnPacket[a];
		if(sum==ReturnPacket[7+Length]){return data;}
		else {return -1;}
	}
	void Close()
	{
		serialClose(fd);	
	}

	int RightFlush(const int Id)
	{
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
	void Reboot(const int Id)
	{
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

	void SetId(const int Id,const int Id2){                   SendPacket_Short(Id,0x00,0x04,0x01,0x01,Id2);}
	void SetReverse(const int Id,const int Enable){           SendPacket_Short(Id,0x00,0x05,0x01,0x01,Enable);}
	int SetBaudRate(const int Id,const int baurate)
	{
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
	void SetReturnDelay(const int Id,const int Time){         SendPacket_Short(Id,0x00,0x07,0x01,0x01,Time);}
	void SetCWLimit(const int Id,const int Angle){            SendPacket_Short(Id,0x00,0x08,0x02,0x01,Angle);}
	void SetCCWLimit(const int Id,const int Angle){           SendPacket_Short(Id,0x00,0x0A,0x02,0x01,Angle);}
	void SetDamper(const int Id,const int Amount){            SendPacket_Short(Id,0x00,0x14,0x01,0x01,Amount);}
	void SetTorqueSilence(const int Id,const int Mode){       SendPacket_Short(Id,0x00,0x16,0x01,0x01,Mode);}
	void SetWarmUpTime(const int Id,const int Time){          SendPacket_Short(Id,0x00,0x17,0x01,0x01,Time);}
	void SetCWComplianceMargin(const int Id,const int Angle){ SendPacket_Short(Id,0x00,0x18,0x01,0x01,Angle);}
	void SetCCWComplianceMargin(const int Id,const int Angle){SendPacket_Short(Id,0x00,0x19,0x01,0x01,Angle);}
	void SetCWComplianceSlope(const int Id,const int Angle){  SendPacket_Short(Id,0x00,0x1A,0x01,0x01,Angle);}
	void SetCCWComplianceSlope(const int Id,const int Angle){ SendPacket_Short(Id,0x00,0x1B,0x01,0x01,Angle);}
	void SetPunch(const int Id,const int Perser){             SendPacket_Short(Id,0x00,0x1C,0x02,0x01,Perser);}
	void SetPosition(const int Id,const int Angle){           SendPacket_Short(Id,0x00,0x1E,0x02,0x01,Angle);}
	void SetTime(const int Id,const int Time){                SendPacket_Short(Id,0x00,0x20,0x02,0x01,Time);}
	void SetMaxTorque(const int Id,const int Parser){         SendPacket_Short(Id,0x00,0x23,0x01,0x01,Parser);}
	void SetTorqueEnable(const int Id,const int Enable){      SendPacket_Short(Id,0x00,0x24,0x01,0x01,Enable);}

	int GetModelNumber(const int Id)		{return ReturnPacket(Id,0x00,2);}
	int GetFirmwareVersion(const int Id)	{return ReturnPacket(Id,0x02,1);}
	int GetId(const int Id)			{return ReturnPacket(Id,0x04,1);}
	int GetReverse(const int Id)		{return ReturnPacket(Id,0x05,1);}
	int GetBaudRate(const int Id)		
	{
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
	int GetReturnDelay(const int Id)		{return ReturnPacket(Id,0x07,1);}
	int GetCWLimit(const int Id)		{return ReturnPacket(Id,0x08,2);}
	int GetCCWLimit(const int Id)		{return ReturnPacket(Id,0x0A,2);}
	int GetTempLimit(const int Id)		{return ReturnPacket(Id,0x0E,2);}
	int GetDamper(const int Id)		{return ReturnPacket(Id,0x14,1);}
	int GetTorqueSilence(const int Id)		{return ReturnPacket(Id,0x16,1);}
	int GetWarmUpTime(const int Id)		{return ReturnPacket(Id,0x17,1);}
	int GetCWComplianceMargin(const int Id)	{return ReturnPacket(Id,0x18,1);}
	int GetCCWCompianceMargin(const int Id)	{return ReturnPacket(Id,0x19,1);}
	int GetCWComplianceSlope(const int Id)	{return ReturnPacket(Id,0x1A,1);}
	int GetCCWComplianceSlope(const int Id)	{return ReturnPacket(Id,0x1B,1);}
	int GetPunch(const int Id)		{return ReturnPacket(Id,0x1C,2);}
	int GetGoalPosition(const int Id)		{return ReturnPacket(Id,0x1E,2);}
	int GetGoalTime(const int Id)		{return ReturnPacket(Id,0x20,2);}
	int GetMaxTorque(const int Id)		{return ReturnPacket(Id,0x23,1);}
	int GetTorqueEnable(const int Id)		{return ReturnPacket(Id,0x24,1);}
	int GetPosition(const int Id)		{return ReturnPacket(Id,0x2A,2);}
	int GetTime(const int Id)			{return ReturnPacket(Id,0x2C,2);}
	int GetSpeed(const int Id)		{return ReturnPacket(Id,0x2E,2);}
	int GetCurrent(const int Id)		{return ReturnPacket(Id,0x30,2);}
	int GetTemp(const int Id)			{return ReturnPacket(Id,0x32,2);}
	int GetVolts(const int Id)		{return ReturnPacket(Id,0x34,2);}
};
