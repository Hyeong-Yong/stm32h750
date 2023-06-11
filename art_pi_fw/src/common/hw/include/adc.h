/*
 * adc.h
 *
 *  Created on: 2021. 2. 12.
 *      Author: baram
 */

#ifndef SRC_COMMON_HW_INCLUDE_ADC_H_
#define SRC_COMMON_HW_INCLUDE_ADC_H_

#ifdef __cplusplus
 extern "C" {
#endif

#include "hw_def.h"
#include "cli.h"

#ifdef _USE_HW_ADC


#define ADC_MAX_CH    HW_ADC_MAX_CH


bool adcInit();
bool adc_InterleavedMode_Open();
bool adcInitInterleaved();
bool adcIsInitInterleaved();
bool adcSingleMeasure();
bool MX_ADC1_Init(void);
bool MX_ADC2_Init(void);


#endif

#ifdef __cplusplus
}
#endif

#endif /* SRC_COMMON_HW_INCLUDE_ADC_H_ */
