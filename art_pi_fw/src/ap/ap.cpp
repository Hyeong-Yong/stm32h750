/*
 * ap.cpp
 *
 *  Created on: Apr 19, 2021
 *      Author: baram
 */




#include "ap.h"
#include "adc.h"

const uint8_t HEADER = 0xF0;



void apInit(void)
{
  cliOpen(_DEF_UART1, 57600);
  //adcInitInterleaved();
  spiOpen(_DEF_SPI1);
  pwmStart(_DEF_PWM1); //PA15, RF GENERATOR TRIGGER IN (Windfreak)
  pwmSycDelay(_DEF_PWM1, 10000); // 10ms delay
  pwmStart(_DEF_PWM2); //PH10, DIGITIZER TRIGGER IN (AlazarTech)
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

    cliMain();

//    sd_state_t sd_state;
//
//    sd_state = sdUpdate();
//    if (sd_state == SDCARD_CONNECTED)
//    {
//      logPrintf("\nSDCARD_CONNECTED\n");
//    }
//    if (sd_state == SDCARD_DISCONNECTED)
//    {
//      logPrintf("\nSDCARD_DISCONNECTED\n");
//    }
  }
}
