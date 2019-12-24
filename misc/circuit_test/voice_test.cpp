#include<stdio.h>
#include "./hedder/voice.h"
#define STR_MAX_LENGTH 100

int main(int argc,char**argv)
{
	char buf_c[STR_MAX_LENGTH];
	voice_c voice;
	sprintf(buf_c,"てすと");
	voice.speak(buf_c);
}
