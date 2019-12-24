// gcc -o io_sample io_sample.cpp ../io_input.cpp -lwiringPi

#include <stdio.h>
#include <unistd.h>
#include <wiringPi.h>
#include "include/io_input.h"

#define SW1_PIN 22
#define SW2_PIN 25
#define SW3_PIN 5
#define SW4_PIN 12
#define SW5_PIN 26
#define SW6_PIN 13
#define SW7_PIN 19

int main(int argv, char **argc)
{
  io_input_c io_input;

  while(1){
    printf("%d %d %d %d %d %d %d\n",
      io_input.get_D_SW1_enable(),
      io_input.get_D_SW2_enable(),
      io_input.get_D_SW3_enable(),
      io_input.get_D_SW4_enable(),
      io_input.get_S_SW1_enable(),
      io_input.get_S_SW2_enable(),
      io_input.get_S_SW3_enable()
    );
  }
}
