/*
 * hw.h
 *
 *  Created on: Dec 6, 2020
 *      Author: baram
 */

#ifndef SRC_HW_HW_H_
#define SRC_HW_HW_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "hw_def.h"


#include "led.h"
#include "uart.h"
#include "cli.h"
#include "button.h"
#include "qspi.h"
#include "flash.h"
#include "sdram.h"
#include "gpio.h"
#include "sd.h"
#include "fatfs.h"
#include "ltdc.h"
#include "lcd.h"
#include "adc.h"
#include "spi.h"
#include "dac8562.h"
#include "pwm.h"

void hwInit(void);


#ifdef __cplusplus
}
#endif

#endif /* SRC_HW_HW_H_ */
