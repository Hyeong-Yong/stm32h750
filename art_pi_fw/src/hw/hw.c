/*
 * hw.c
 *
 *  Created on: Dec 6, 2020
 *      Author: baram
 */


#include "hw.h"


extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc2;
extern DMA_HandleTypeDef hdma_adc1;



void hwInit(void)
{
  bspInit();

  cliInit();

  ledInit();
  uartInit();
  //uartOpen(_DEF_UART1, 57600);

  logPrintf("\n\n");
  logPrintf("[ Firmware Begin... ]\r\n");
  logPrintf("Booting..Clock\t\t: %d Mhz\r\n", (int)HAL_RCC_GetSysClockFreq()/1000000);

  gpioInit();
  buttonInit();

  sdramInit();
  qspiInit();
  flashInit();

  sdInit();
  fatfsInit();


  spiInit();
  dac8562_init();

  //lcdInit();
  adcInit();


}
