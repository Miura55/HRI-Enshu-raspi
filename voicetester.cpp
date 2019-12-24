// g++ -o voicevolt voicevolt.cpp adc.cpp voice.cpp -lwiringPi -lpthread -lm

#include <cstdio>
#include <cstdlib>
#include <wiringPi.h>

#include "./include/adc.h"
#include "./include/voice.h"

#define FILE_DIR (char*)"/tmp"

int main(int argc, char *argv[])
{
    adc_c adc;
    voice_c voice;
    char str[512];
    
    sprintf(str, "バッテリーの電圧は %.2f ぼるとです", adc.get(VDD)*4.0);
    voice.speak(FILE_DIR, str);
    voice.speak_join();

    exit(EXIT_SUCCESS);
}
