/*
 * ap.cpp
 *
 *  Created on: Apr 19, 2021
 *      Author: baram
 */




#include "ap.h"
#include "adc.h"
#include "pwm.h"


void apInit(void)
{
  //cliOpen(_DEF_UART1, 57600);
  //adcInitInterleaved();
  uartOpen(_DEF_UART1, 57600);
  spiOpen(_DEF_SPI1);
  //pwmStart(_DEF_PWM1); //PA15, RF GENERATOR TRIGGER IN (Windfreak)
  //pwmSet_SycDelay(_DEF_PWM1, 100); // 100us delay
  //pwmStart(_DEF_PWM2); //PH10, DIGITIZER TRIGGER IN (AlazarTech)
  spiOpen(_DEF_SPI2);
}

void apMain(void)
{
  uint32_t pre_time;


  pre_time = millis();
  while(1)
  {
    if (millis()-pre_time >= 5000)
    {
      pre_time = millis();
      ledToggle(_DEF_LED1);
    }

    //cliMain();


    // pulse : 20us, step time : 0.1 ms, interval : 10 ms
    triggerReceivePacket(_DEF_UART1);

  }
}
