/*
 * pwm.h
 *
 *  Created on: 2021. 2. 14.
 *      Author: baram
 */

#ifndef SRC_COMMON_HW_INCLUDE_PWM_H_
#define SRC_COMMON_HW_INCLUDE_PWM_H_

#ifdef __cplusplus
extern "C" {
#endif


#include "hw_def.h"


#include "hw_def.h"

#define PWM_MAX_CH	HW_PWM_MAX_CH


#ifdef _USE_HW_PWM


bool pwmInit(void);
bool pwmBegin(uint8_t ch, uint32_t period, uint32_t pulse, uint32_t prescaler);
bool pwmDeinit(uint8_t ch);
void pwmStart(uint8_t ch);
bool pwmStop(uint8_t ch);
void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

#endif


#ifdef __cplusplus
}
#endif

#endif /* SRC_COMMON_HW_INCLUDE_PWM_H_ */
