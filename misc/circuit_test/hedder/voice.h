#pragma once
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>
	
#define PATH_SCR "sh /home/pi/key_ws/circit_test/hedder/open_jtalk.sh"
#define STR_MAX_LENGTH 100
class voice_c
{
private:
	char buf_c[STR_MAX_LENGTH];
public:
	voice_c()
	{
	};
	void speak(char* word);
};
void voice_c::speak(char* word)
{
	sprintf(buf_c,"%s %s",PATH_SCR,word);
	system(buf_c);
}
