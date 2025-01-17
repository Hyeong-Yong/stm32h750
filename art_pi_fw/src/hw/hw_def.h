/*
 * hw_def.h
 *
 *  Created on: Dec 6, 2020
 *      Author: baram
 */

#ifndef SRC_HW_HW_DEF_H_
#define SRC_HW_HW_DEF_H_


#include "def.h"
#include "bsp.h"



#define _USE_HW_SD

#define _USE_HW_QSPI
#define _USE_HW_SPI
#define      HW_SPI_MAX_CH			2

#define _USE_HW_FLASH
#define _USE_HW_FATFS

#define _USE_HW_PWM
#define      HW_PWM_MAX_CH			2


#define _USE_HW_ADC
#define 	 HW_ADC_MAX_CH			1

#define _USE_HW_LED
#define      HW_LED_MAX_CH          2



#define _USE_HW_UART
#define      HW_UART_MAX_CH         2

#define _USE_HW_CLI
#define      HW_CLI_CMD_LIST_MAX    24
#define      HW_CLI_CMD_NAME_MAX    16
#define      HW_CLI_LINE_HIS_MAX    4
#define      HW_CLI_LINE_BUF_MAX    64

#define _USE_HW_BUTTON
#define      HW_BUTTON_MAX_CH       1

#define _USE_HW_SDRAM
#define      HW_SDRAM_MEM_ADDR      0xC0000000
#define      HW_SDRAM_MEM_SIZE      (32*1024*1024)

#define _USE_HW_GPIO
#define      HW_GPIO_MAX_CH         3

#define _USE_HW_LCD
#define _USE_HW_LTDC
#define      HW_LCD_WIDTH           800
#define      HW_LCD_HEIGHT          480


#define _PIN_GPIO_SDCARD_DETECT     0

#define _PIN_GPIO_DAC8562_SYNC      2




#endif /* SRC_HW_HW_DEF_H_ */
