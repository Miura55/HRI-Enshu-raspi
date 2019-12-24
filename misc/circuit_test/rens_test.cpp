#include <stdio.h>
#include <unistd.h>
#include <wiringPi.h>

#define MAIN_ENA 6
#define IRIS_ENA 16
#define ZOOM_ENA 20
#define FOCUS_ENA 21



int main(int argv, char **argc){
	int i;
	
	if (wiringPiSetupGpio() == -1) printf("Connect Error\n");
  
	pinMode(MAIN_ENA, OUTPUT);
	pinMode(IRIS_ENA, OUTPUT);
	pinMode(ZOOM_ENA, OUTPUT);
	pinMode(FOCUS_ENA, OUTPUT);

	digitalWrite(MAIN_ENA,0);
	digitalWrite(IRIS_ENA,0);
	digitalWrite(ZOOM_ENA,0);
	digitalWrite(FOCUS_ENA,0);
	
	printf("start\n");

	printf("IRIS\n");
	digitalWrite(IRIS_ENA,1);
	for(i=0;i<3;i++){
		usleep(1.0*1000.0*1000.0);
		digitalWrite(MAIN_ENA,1);
		usleep(1.0*1000.0*1000.0);
		digitalWrite(MAIN_ENA,0);
	}
	digitalWrite(IRIS_ENA,0);
	
	printf("ZOOM\n");
	digitalWrite(ZOOM_ENA,1);
	for(i=0;i<3;i++){
		usleep(1.0*1000.0*1000.0);
		digitalWrite(MAIN_ENA,1);
		usleep(1.0*1000.0*1000.0);
		digitalWrite(MAIN_ENA,0);
	}
	digitalWrite(ZOOM_ENA,0);
	
	printf("FOCUS\n");
	digitalWrite(FOCUS_ENA,1);
	for(i=0;i<3;i++){
		usleep(1.0*1000.0*1000.0);
		digitalWrite(MAIN_ENA,1);
		usleep(1.0*1000.0*1000.0);
		digitalWrite(MAIN_ENA,0);
	}
	digitalWrite(FOCUS_ENA,0);
		
	printf("end\n");
	digitalWrite(MAIN_ENA,0);
}
