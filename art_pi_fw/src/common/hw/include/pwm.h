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


#define STX	0xF0


enum state
{
  STATE_HEADER=0,
  STATE_LENGTH,
  STATE_INST,
  STATE_PARAM_1,
  STATE_PARAM_2,
  STATE_CHECK,
};
enum index
{
  INDEX_HEADER=0,
  INDEX_LENGTH,
  INDEX_INST,
  INDEX_PARAM_1,
  INDEX_PARAM_2,
  INDEX_CHECK,
};

enum
{
  PWM_INST_STATUS = 0x01,
  PWM_INST_READ = 0x02,
  PWM_INST_WRITE = 0x03,
};


typedef struct
{
  uint8_t header;
  uint8_t length;
  uint8_t inst;
  uint8_t check;
  uint16_t param;
} pwm_packet_t;


bool pwmInit(void);
bool pwmBegin(uint8_t ch, uint32_t period, uint32_t pulse, uint32_t prescaler);
bool pwmDeinit(uint8_t ch);
void pwmStart(uint8_t ch);
bool pwmStop(uint8_t ch);
void pwmSet_SycDelay(uint8_t ch, uint32_t delay);
void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);
uint32_t pwmGet_SycDelay(uint8_t ch);
void pwmRun_ODMR();

bool pwmProcessPKT(uint8_t rx_data);
bool ReceivePacket(uint8_t ch);
bool checksumPacket();

#endif


#ifdef __cplusplus
}
#endif

#endif /* SRC_COMMON_HW_INCLUDE_PWM_H_ */
