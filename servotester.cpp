// How to Run
// g++ -o servotester servotester.cpp io_input.cpp rs405cb.cpp -lwiringPi -lpthread -lm

#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <unistd.h>
#include <wiringPi.h>

#include "./include/io_input.h"
#include "./include/rs405cb.h"


const int MOTOR_ID = 1;
#define FILE_DIR (char*)"/tmp"

void motor_test(void);

int main(int argc, char *argv[])
{
    printf("\nmotor_test()\n\n");
    motor_test();

    exit(EXIT_SUCCESS);
}

void motor_test(void)
{
    int id = MOTOR_ID;
    rs405cb_c rs405cb;
	
    if (wiringPiSetupGpio() == -1) printf("Connect Error\n");
	
    printf("connect %d\n", id);
    rs405cb.connect(ENABLEPIN, BAUDRATE);

    printf("init\n");
    rs405cb.SetTime(id, 100);
    rs405cb.SetTorqueEnable(id, 1);

    printf("set postion 0 degree\n");
    rs405cb.SetPosition(id, 0.0);
    sleep(1);

    printf("set postion 180 degree\n");
    rs405cb.SetPosition(id, 1800.0);
    sleep(3);

    printf("set postion 0 degree\n");
    rs405cb.SetPosition(id, 0.0);
    sleep(1);

    printf("set postion -180 degree\n");
    rs405cb.SetPosition(id, -1800.0);
    sleep(3);

    printf("set postion 0 degree\n");
    rs405cb.SetPosition(id, 0.0);
    sleep(3);

    rs405cb.SetTorqueEnable(id, 0);
}
