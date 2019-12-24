/*合成音声プログラム
 * ・関数説明
 * 		speak(char* word):wordの文字列を音声合成で再生する（別スレッド）
 * 		speak_file(char* path):pathの音声ファイルを再生する（別スレッド）
 * 		speak_join():実行中のスレッドの終了を待つ
 */

#ifndef VOICEH
#define VOICEH

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
	
#define STR_MAX_LENGTH 512

class voice_c
{
private:
	pthread_t thd_handler;
	char buf_c[STR_MAX_LENGTH];
	static void *run_thd(void*data){
		system((char*)data);
	}
public:
	voice_c(){};
	void speak(char* dir, const char* word);
	void speak_file(char* dir, char* path);
	void speak_join();
};

#endif // VOICEH
