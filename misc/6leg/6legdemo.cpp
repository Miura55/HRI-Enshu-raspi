// gcc -o 6legdemo 6legdemo.cpp adc.cpp io_input.cpp motion.cpp mpu6050.cpp rs405cb.cpp voice.cpp -lwiringPi -lm -lpthread

#include <stdio.h>
#include <wiringPi.h>
#include <unistd.h>

#include "include/voice.h"
#include "include/io_input.h"
#include "include/motion.h"
#include "include/adc.h"

#define FILE_DIR (char*)"include/speach_file"

void demo(motion_c motion,voice_c voice){
  int i,j,sita;
  double z = 100.0,r=300.0;
  
  motion.set_mot_enable(1);
  //start
  motion.init_pose(r,z,100.0);

  //R   # sita -> theta
  for(sita=0;sita<800;sita+=30)motion.walk_tri_pot_init(z,r,1.0,sita);
  for(sita=0;sita<800;sita+=5)motion.walk_tri_pot(z,0.0,r,0.0,20.0,1.0,sita);
  
  //greeting
  motion.init_pose(r,z,50.0);
  for(sita=0;sita<800;sita+=10)motion.greeting(z,r,100,1.0,sita);
  for(sita=0;sita<800;sita+=10)motion.greeting(z,r,-100,1.0,sita);
  
  //L x 2
  for(sita=0;sita<800;sita+=20)motion.walk_tri_pot_init(z,r,1.0,sita);
  for(sita=0;sita<800*2;sita+=5)motion.walk_tri_pot(z,0.0,r,0.0,-20.0,1.0,sita);
  
  //greeting
  motion.init_pose(r,z,50.0);
  for(sita=0;sita<800;sita+=10)motion.greeting(z,r,100,1.0,sita);
  for(sita=0;sita<800;sita+=10)motion.greeting(z,r,-100,1.0,sita);
  
  //R
  for(sita=0;sita<800;sita+=30)motion.walk_tri_pot_init(z,r,1.0,sita);
  for(sita=0;sita<800;sita+=5)motion.walk_tri_pot(z,0.0,r,0.0,20.0,1.0,sita);
  
  //greeting
  motion.init_pose(r,z,50.0);
  for(sita=0;sita<800;sita+=10)motion.greeting(z,r,100,1.0,sita);
  for(sita=0;sita<800;sita+=10)motion.greeting(z,r,-100,1.0,sita);
  sleep(1.0);
  
  //speech1
  motion.init_pose(r,z,100.0);
  for(sita=0;sita<800;sita+=50)motion.saying(z,r,50,1.0,sita);
  voice.speak_file(FILE_DIR,(char*)"speach1_1.wav");//はじめまして わたくしはちゅうぶだいがくでかいはつされたろくあしほこうがたろぼっとです
  for(sita=0;sita<800;sita+=7)motion.saying_roll(z,r,50,40.0,1.0,sita);
  for(sita=0;sita<800;sita+=7)motion.saying_roll(z,r,50,-40.0,1.0,sita);
  voice.speak_join();
  voice.speak_file(FILE_DIR,(char*)"speach1_2.wav");//きょうはすこしばかりじこしょうかいをしたいとおもいます
  for(sita=0;sita<800;sita+=7)motion.saying_roll(z,r,50,40.0,1.0,sita);
  for(sita=0;sita<800;sita+=7)motion.saying_roll(z,r,50,-40.0,1.0,sita);
  voice.speak_join();
  for(sita=0;sita<800;sita+=50)motion.saying(z,r,-50,1.0,sita);
  for(sita=0;sita<800;sita+=10)motion.greeting(z,r,100,1.0,sita);
  for(sita=0;sita<800;sita+=10)motion.greeting(z,r,-100,1.0,sita);

  //speech2
  voice.speak_file(FILE_DIR,(char*)"speach2_1.wav");//わたくしは　かくかんせつのもーたをせいぎょうることでさまざまなうごきをさいげんできます。
  voice.speak_join();
  voice.speak_file(FILE_DIR,(char*)"speach2_2.wav");//たとえば
  voice.speak_join();
  voice.speak_file(FILE_DIR,(char*)"speach2_3.wav");//あしぶみをしたり
  for(sita=0;sita<800;sita+=10)motion.foot_step(z,r,50,1.0,sita);
  voice.speak_join();
  voice.speak_file(FILE_DIR,(char*)"speach2_4.wav");//こんななめらかなうごきもさいげんできます
  motion.body_wave(z,r,50,30.0,0.0);
  for(sita=0;sita<=360;sita+=5)motion.body_wave(z,r,80,1.0,sita);
  for(sita=360;sita>0;sita-=5)motion.body_wave(z,r,80,1.0,sita);
  
  motion.init_pose(r,z,50.0);
  motion.body_roll(z,r,50,30.0,0.0);
  for(sita=0;sita<=360;sita+=5)motion.body_roll(z,r,50,1.0,sita);
  for(sita=360;sita>0;sita-=5)motion.body_roll(z,r,50,1.0,sita);
  voice.speak_join();

  voice.speak_file(FILE_DIR,(char*)"speach2_5.wav");//ほこうだってかのうですよ  ほかにもからだじゅうにおんどせんさやじゃいろせんさなどのせんさをなんこもとりつけています
  motion.walk_tri_pot_init(z,r,100.0,0.0);
  for(sita=0;sita<800;sita+=30)motion.walk_tri_pot_init(z,r,1.0,sita);
  for(sita=0;sita<800*2;sita+=5)motion.walk_tri_pot(z,50.0,r,45.0,0.0,1.0,sita);
  for(sita=0;sita<800*2;sita+=5)motion.walk_tri_pot(z,50.0,r,-45.0,0.0,1.0,sita);
  for(sita=0;sita<800*3;sita+=5)motion.walk_tri_pot(z,50.0,r,180.0,0.0,1.0,sita);
  voice.speak_join();

  //speach3
  motion.init_pose(r,z,100.0);
  for(sita=0;sita<800;sita+=50)motion.saying(z,r,50,1.0,sita);
  voice.speak_file(FILE_DIR,(char*)"speach3_1.wav");//げんざいは　ぷろぐらむされたもーしょんのじっこうしかできませんが　しょうらいはふせいちでかつやくするようけんきゅうちゅうです   
  voice.speak_join(); 
  motion.init_pose(r,z,100.0);
  voice.speak_file(FILE_DIR,(char*)"speach3_2.wav");//おうえんをよろしくおねがいします  
  for(sita=0;sita<800;sita+=10)motion.greeting(z,r,100,1.0,sita);
  for(sita=0;sita<800;sita+=10)motion.greeting(z,r,-100,1.0,sita);
  voice.speak_join(); 
    
  //speach4
  motion.init_pose(r,z,100.0);
  voice.speak_file(FILE_DIR,(char*)"speach4_1.wav");//それではみじかいあいだでしたがでもをしゅうりょうさせていただきます
  voice.speak_join(); 
  
  motion.end_pose(100.0);
  motion.set_mot_enable(0);
}


int main(int argv, char **argc){
    int i;
    io_input_c io;
    voice_c voice;
    motion_c motion;
    adc_c adc;
    char buf_c[500];
    
    voice.speak_file(FILE_DIR,(char*)"start.wav");
    
    //init pose
    motion.set_mot_enable(1);
    motion.end_pose(100.0);
    motion.set_mot_enable(0);
    
    voice.speak_join();
    while(1){
      if(io.get_D_SW1_enable() == 0){
	voice.speak_file(FILE_DIR,(char*)"shutdown1.wav");
	for(i=0;i<=4000;i++){
	  if(i==1000){voice.speak_file(FILE_DIR,(char*)"3.wav");}
	  else if(i==2000){voice.speak_file(FILE_DIR,(char*)"2.wav");}
	  else if(i==3000){voice.speak_file(FILE_DIR,(char*)"1.wav");}
	  else if(i==4000){
	    voice.speak_file(FILE_DIR,(char*)"shutdown2.wav");
	    voice.speak_join();
	    system("sudo shutdown 0");
	  }
	  
	  if(io.get_D_SW1_enable() == 1){
	    voice.speak_file(FILE_DIR,(char*)"shutdown3.wav");
	    voice.speak_join();
	    break;
	  }
	  voice.speak_join();
	}
      }
      
      if(io.get_D_SW2_enable() != 1){
	sprintf(buf_c,"でんげんでんあつは%5.2fぼるとです",adc.get(2)*4);
	voice.speak(FILE_DIR,buf_c);
	motion.set_mot_enable(1);
	motion.end_pose(100.0);
	motion.set_mot_enable(0);
	voice.speak_join();
	
      }
      
      if(io.get_D_SW3_enable() != 1){
	voice.speak_file(FILE_DIR,(char*)"demo1.wav");
	voice.speak_join(); 
	for(i=0;i<=4000;i++){
	  if(i==1000){voice.speak_file(FILE_DIR,(char*)"3.wav");}
	  else if(i==2000){voice.speak_file(FILE_DIR,(char*)"2.wav");}
	  else if(i==3000){voice.speak_file(FILE_DIR,(char*)"1.wav");}
	  else if(i==4000){	    
	    voice.speak_file(FILE_DIR,(char*)"demo2.wav");
	    voice.speak_join();
	    demo(motion,voice);
	    break;
	  } 
	  
	  if(io.get_D_SW3_enable() == 1){
	    voice.speak_file(FILE_DIR,(char*)"demo3.wav");
	    voice.speak_join();
	    break;
	  }
	  voice.speak_join();
	}
      }
      
      if(io.get_D_SW4_enable() != 1){
	voice.speak_file(FILE_DIR,(char*)"system_down1.wav");
	for(i=0;i<=4000;i++){
	  if(i==1000){voice.speak_file(FILE_DIR,(char*)"3.wav");}
	  else if(i==2000){voice.speak_file(FILE_DIR,(char*)"2.wav");}
	  else if(i==3000){voice.speak_file(FILE_DIR,(char*)"1.wav");}
	  else if(i==4000){	    
	    voice.speak_file(FILE_DIR,(char*)"system_down2.wav");
	    voice.speak_join();
	    return 0;
	  }
	  
	  if(io.get_D_SW4_enable() == 1){
	    voice.speak_file(FILE_DIR,(char*)"system_down3.wav");
	    voice.speak_join();
	    break;
	  }
	  voice.speak_join();
	}
      }
      usleep(1.0);
    }
}
