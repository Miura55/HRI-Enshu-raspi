/*合成音声プログラム
 * ・関数説明
 * 		speak(char* word):wordの文字列を音声合成で再生する（別スレッド）
 * 		speak_file(char* path):pathの音声ファイルを再生する（別スレッド）
 * 		speak_join():実行中のスレッドの終了を待つ
 */

#include "include/voice.h"

/*
const char DICPATH[] = "/usr/local/share/open_jtalk/open_jtalk_dic_utf_8-1.11";
const char VOICEPATH[] = "/usr/local/share/hts_voice/hts_voice_nitech_jp_atr503_m001-1.05/nitech_jp_atr503_m001.htsvoice";
*/

const char DICPATH[] = "/var/lib/mecab/dic/open-jtalk/naist-jdic";
const char VOICEPATH[] = "/home/pi/MMDAgent_Example-1.7/Voice/mei/mei_normal.htsvoice";

void voice_c::speak(char* dir, const char* word)
{
    sprintf(this -> buf_c, "echo %s | open_jtalk -x %s -m %s -ow %s/buf.wav -r 1.0",
	    word, DICPATH, VOICEPATH, dir);
    system(this -> buf_c);
    sprintf(this -> buf_c, "aplay --quiet %s/buf.wav", dir);
    pthread_create(&this -> thd_handler, NULL, this -> run_thd, this -> buf_c);
}

void voice_c::speak_file(char* dir, char* file)
{
    sprintf(this -> buf_c, "aplay --quiet %s/%s", dir, file);
    pthread_create(&this -> thd_handler, NULL, voice_c::run_thd, (void*)this -> buf_c);
}

void voice_c::speak_join()
{
    pthread_join(this -> thd_handler, NULL);
}

/*
void voice_c::speak(char* dir, char* word)
{
	sprintf(this -> buf_c, "echo %s | open_jtalk -x /var/lib/mecab/dic/open-jtalk/naist-jdic \
			-m /home/pi/MMDAgent_Example-1.7/Voice/mei/mei_normal.htsvoice \
			-ow %s/buf.wav -r 1.0", word, dir);
	system(this -> buf_c);
	sprintf(this -> buf_c, "aplay --quiet %s/buf.wav", dir);
	pthread_create(&this -> thd_handler, NULL, this -> run_thd, buf_c);
}

void voice_c::speak_file(char* dir, char* path)
{
	sprintf(this -> buf_c, "aplay --quiet %s/%s", dir, path);
	pthread_create(&this -> thd_handler, NULL, voice_c::run_thd, (void*)this -> buf_c);
}

void voice_c::speak_join()
{
	pthread_join(this -> thd_handler, NULL);
}
*/
