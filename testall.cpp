// g++ -o testall testall.cpp adc.cpp motion.cpp io_input.cpp mpu6050.cpp rs405cb.cpp voice.cpp -lwiringPi -lpthread -lm

// Raspbian
// cc -o TARGET TARGET.cpp  -lwiringPi -lm
// g++ -o TARGET TARGET.cpp -lwiringPi -lm

// Ubuntu
// cc -o TARGET TARGET.cpp  -lwiringPi -lpthread -lcrypt -lrt -lm
// g++ -o TARGET TARGET.cpp -lwiringPi -lpthread -lcrypt -lrt -lm

#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <unistd.h>
#include <wiringPi.h>

#include "./include/adc.h"
#include "./include/io_input.h"
#include "./include/rs405cb.h"
#include "./include/mpu6050.h"
#include "./include/voice.h"

const int MOTOR_ID = 1;
#define FILE_DIR (char*)"/tmp"
//#define FILE_DIR (char*)"./include/speach_file" // old

void adc_test(void);
void io_test(void);
void motor_test(void);
void mpu6050_test(void);
void voice_test(void);

int main(int argc, char *argv[])
{
    adc_c adc;
    voice_c voice;
    char str[512];
    
    sprintf(str, "バッテリーの電圧は %.2f ぼるとです", adc.get(VDD)*4.0);
//  printf("%s", str);
//  fflush(stdout);
    voice.speak(FILE_DIR, str);
    voice.speak_join();

    voice.speak(FILE_DIR, "AD 変換のテスト");
    printf("\nadc_test()\n\n");
    adc_test();

    voice.speak(FILE_DIR, "ディジタル IO のテスト");
    printf("\nio_test()\n\n");
    io_test();

    voice.speak(FILE_DIR, "サーボモータのテスト");
    printf("\nmotor_test()\n\n");
    motor_test();

    voice.speak(FILE_DIR, "加速度センサのテスト");
    printf("\nmpu6050_test()\n\n");
    mpu6050_test();

    voice.speak(FILE_DIR, "音声合成と出力のテスト");
    printf("\nvoice_test()\n\n");
    voice.speak_join();

    voice_test();

    exit(EXIT_SUCCESS);
}

void adc_test(void)
{
    int i = 0;
    adc_c adc;

    while (i++ < 10) {
	printf("VDD: %f [V]\n",adc.get(VDD)*4.0);
	sleep(1);
    }
}

void io_test(void)
{
    int i = 0;
    io_input_c io_input;

    while (i++ < 10) {
        printf("%d %d %d %d %d %d %d\n",
	       io_input.get_T_SW1_enable(),
	       io_input.get_T_SW2_enable(),
	       io_input.get_T_SW3_enable(),
	       io_input.get_T_SW4_enable(),
	       io_input.get_S_SW1_enable(),
	       io_input.get_S_SW2_enable(),
	       io_input.get_S_SW3_enable());
        sleep(1);
    }
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

    printf("set postion 60 degree\n");
    rs405cb.SetPosition(id, 600.0);
    sleep(3);

    printf("set postion -60 degree\n");
    rs405cb.SetPosition(id, -600.0);
    sleep(3);

    printf("set postion 0 degree\n");
    rs405cb.SetPosition(id, 0.0);
    sleep(3);

    rs405cb.SetTorqueEnable(id, 0);
}

void mpu6050_test(void)
{
    int i = 0;
    mpu6050_c mpu6050;

    while (i++ < 10) {
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
}

void voice_test(void)
{
    voice_c voice;
    const char str[] = "これはスピーチ機能のテストです あいうえお abcde 12345.67";
    
    printf("%s\n", str);
    fflush(stdout);
    voice.speak(FILE_DIR, str);
    //voice.speak_file(FILE_DIR, (char*)"start.wav");
    voice.speak_join();

    voice.speak(FILE_DIR, "私の名前は中野です 大変待たせてごめんなさい");
    voice.speak_join();
}
