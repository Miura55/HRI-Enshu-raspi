// gcc -o voice_sample voice_sample.cpp ../voice.cpp -lwiringPi -lpthread

#include<stdio.h>
#include "include/voice.h"

#define FILE_DIR (char*)"include/speach_file"

int main(int argc, char *argv[])
{
    voice_c voice;
    
    voice.speak(FILE_DIR, (char*)"あいうえお abcde 12345");
    //voice.speak_file(FILE_DIR,(char*)"start.wav");
    voice.speak_join();
    return 0;
}
