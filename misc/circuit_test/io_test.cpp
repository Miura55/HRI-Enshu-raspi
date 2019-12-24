#include <stdio.h>
#include <unistd.h>
#include <wiringPi.h>

#define SW1_PIN 22
#define SW2_PIN 25
#define SW3_PIN 5
#define SW4_PIN 12
#define SW5_PIN 26
#define SW6_PIN 13
#define SW7_PIN 19


int main(int argv, char **argc){
  int i,sw_enable[7];
	
  if (wiringPiSetupGpio() == -1) printf("Connect Error\n");
  
  pinMode(SW1_PIN, INPUT);
  pinMode(SW2_PIN, INPUT);
  pinMode(SW3_PIN, INPUT);
  pinMode(SW4_PIN, INPUT);
  pinMode(SW5_PIN, INPUT);
  pinMode(SW6_PIN, INPUT);
  pinMode(SW7_PIN, INPUT);

  while(1){
	  sw_enable[0] = digitalRead(SW1_PIN);
	  sw_enable[1] = digitalRead(SW2_PIN);
	  sw_enable[2] = digitalRead(SW3_PIN);
	  sw_enable[3] = digitalRead(SW4_PIN);
	  sw_enable[4] = digitalRead(SW5_PIN);
	  sw_enable[5] = digitalRead(SW6_PIN);
	  sw_enable[6] = digitalRead(SW7_PIN);
	  
	  for(i=0;i<7;i++)printf("%d",sw_enable[i]);
	  printf("\n");
	  usleep(1 * 1000.0);
  }
}
