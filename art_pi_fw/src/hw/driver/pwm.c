/*
 * timer.c
 *
 *  Created on: 2024. 4. 25.
 *      Author: User
 */


#include "cli.h"
#include "pwm.h"
#include "led.h"
#include "uart.h"

#ifdef _USE_HW_PWM

#ifdef _USE_HW_CLI
static void cliPWM(cli_args_t *args);

#endif


//1. init 초기화 (start)
//2. 횟수 : ( (end - start) / interval ) +1

//Config
//1. 딜레이 (pwmSycDelay (_PDEF_PWM1, X)
//2. 횟수 (uint32 NumOfFreq)
//Run
//3. 실행


enum instruction
  {
    INST_SET_TRIGGER_DELAY    = 0x01,
    INST_SET_TRIGGER_COUNT    = 0x02,
    INST_INIT_TRIGGER         = 0x03,
    INST_START_TRIGGER        = 0x04,
  };

typedef struct
{
  TIM_HandleTypeDef* h_tim;
  uint32_t channel;
  uint32_t period;
  uint32_t prescaler;
  uint32_t pulse;

  bool is_timeInit;
  bool is_pwmInit;
} pwm_t;


const uint8_t UART_PWM = _DEF_UART1;
#define PKT_BUF_LENGH 9 // header[1]+LENGTH[1]+INST[1]+PARAM[4]+CRC[2]
typedef struct
{
	  uint32_t period;
	  uint32_t delay;
	  uint32_t total_TriggerCount;
	  uint32_t current_TriggerCount;

	  uint32_t param_index;
	  uint32_t state;
	  uint32_t pre_time;
	  uint8_t packet_buf[PKT_BUF_LENGH];
} pwm_status_t;


TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim5;

pwm_packet_t pwm_packet;
pwm_status_t pwm_status = {10000, 10000, 10, 1, 0,0};
static pwm_t pwm_tbl[PWM_MAX_CH] =
    {
     {&htim2, TIM_CHANNEL_1, 1000000, 240,   10}, //PA15, RF GENERATOR TRIGGER IN (Windfreak)
     {&htim5, TIM_CHANNEL_1, 1000000, 240,   10}, //PH10, DIGITIZER TRIGGER IN (AlazarTech)
	 // 1000000 = 1 sec, 1000 = 1ms, 1 = 1 us
 	 {&htim5, TIM_CHANNEL_2, 1000000, 240,   10000}, //PA1,
    };

void pwmSet_TotalCountFreq(uint32_t total_TriggerCount){
	pwm_status.total_TriggerCount = total_TriggerCount;
}

void pwmSet_SycDelay(uint8_t ch, uint32_t delay)
{
  pwm_t* p_pwm = &pwm_tbl[ch];
  // 1 clock is delayed when using encapulated function. To synchronize, add 1 clock
  p_pwm->h_tim->Instance->CNT = delay+1;
}

uint32_t pwmGet_SycDelay(uint8_t ch)
{
  pwm_t* p_pwm = &pwm_tbl[ch];
  // 1 clock is delayed when using encapulated function. To synchronize, add 1 clock
   return p_pwm->h_tim->Instance->CNT-1;
}



bool triggerReceivePacket(uint8_t ch)
{
  bool ret = false;

  if (ch >= PWM_MAX_CH) return false;

  pwm_t *p_pwm = &pwm_tbl[ch];

  uint8_t rx_data;

  if (p_pwm->is_pwmInit == false)
    {
      return ret;
    }

  pwm_status.pre_time=millis();

  while (uartAvailable(UART_PWM)>0){
      rx_data = uartRead(UART_PWM);
      ret = pwmProcessPKT(rx_data);

      if (ret == true)
	{
	  break;
	}

      if (millis() - pwm_status.pre_time >= 50)
	{
	  break;
	}

  }
  return ret;
}

bool pwmProcessPKT(uint8_t rx_data)
{
  bool ret =false;

  if ( millis() - pwm_status.pre_time > 100)
    {
	  pwm_status.state = STATE_HEADER;
	  pwm_status.param_index=0;
    }
  pwm_status.pre_time= millis();

  switch (pwm_status.state)
  {
    case STATE_HEADER:

      if (rx_data == STX)
	{
      pwm_status.packet_buf[INDEX_HEADER]= rx_data;
      pwm_status.state = STATE_LENGTH;
	}
      break;
    case STATE_LENGTH:
    	pwm_status.packet_buf[INDEX_LENGTH]= rx_data;
    	pwm_status.state = STATE_INST;
      break;
    case STATE_INST:
    	pwm_status.packet_buf[INDEX_INST]= rx_data;
    	pwm_status.state = STATE_PARAM;
      break;
    case STATE_PARAM:
        	pwm_status.packet_buf[INDEX_PARAM_1 + pwm_status.param_index] = rx_data;
        	pwm_status.param_index++;

    	if (pwm_status.param_index == 4){
        	pwm_status.state = STATE_CRC_L;
            pwm_status.param_index=0;
    		}
      break;
    case STATE_CRC_L:
        pwm_status.packet_buf[INDEX_CRC_L] = rx_data;
    	pwm_status.state= STATE_CRC_H;
    	break;
    case STATE_CRC_H:
      pwm_status.packet_buf[INDEX_CRC_H] = rx_data;

      pwm_packet.crc  = (uint16_t)pwm_status.packet_buf[INDEX_CRC_L] <<0;
      pwm_packet.crc |= (uint16_t)pwm_status.packet_buf[INDEX_CRC_H] <<8;
      uint16_t crc = computeCrc(0, (uint8_t*)pwm_status.packet_buf, PKT_BUF_LENGH);
      if (crc == pwm_packet.crc)
      {
      pwm_packet.header = pwm_status.packet_buf[INDEX_HEADER];
      pwm_packet.length = pwm_status.packet_buf[INDEX_LENGTH];
      pwm_packet.inst   = pwm_status.packet_buf[INDEX_INST];
      pwm_packet.param  = pwm_status.packet_buf[INDEX_PARAM_1] << 0;
      pwm_packet.param |= pwm_status.packet_buf[INDEX_PARAM_2] << 8;
      pwm_packet.param  = pwm_status.packet_buf[INDEX_PARAM_3] << 16;
      pwm_packet.param |= pwm_status.packet_buf[INDEX_PARAM_4] << 24;
	  ret=true;
      }
      pwm_status.state = STATE_HEADER;
      pwm_status.param_index=0;
      DoInst(pwm_packet.inst);
      break;
  }
  return ret;
}

uint16_t computeCrc(uint16_t crc_accum, uint8_t *data_blk_ptr, uint16_t data_blk_size)
{
    uint16_t  i, j;
    const uint16_t  crc_table[256] = {
        0x0000, 0x8005, 0x800F, 0x000A, 0x801B, 0x001E, 0x0014, 0x8011,
        0x8033, 0x0036, 0x003C, 0x8039, 0x0028, 0x802D, 0x8027, 0x0022,
        0x8063, 0x0066, 0x006C, 0x8069, 0x0078, 0x807D, 0x8077, 0x0072,
        0x0050, 0x8055, 0x805F, 0x005A, 0x804B, 0x004E, 0x0044, 0x8041,
        0x80C3, 0x00C6, 0x00CC, 0x80C9, 0x00D8, 0x80DD, 0x80D7, 0x00D2,
        0x00F0, 0x80F5, 0x80FF, 0x00FA, 0x80EB, 0x00EE, 0x00E4, 0x80E1,
        0x00A0, 0x80A5, 0x80AF, 0x00AA, 0x80BB, 0x00BE, 0x00B4, 0x80B1,
        0x8093, 0x0096, 0x009C, 0x8099, 0x0088, 0x808D, 0x8087, 0x0082,
        0x8183, 0x0186, 0x018C, 0x8189, 0x0198, 0x819D, 0x8197, 0x0192,
        0x01B0, 0x81B5, 0x81BF, 0x01BA, 0x81AB, 0x01AE, 0x01A4, 0x81A1,
        0x01E0, 0x81E5, 0x81EF, 0x01EA, 0x81FB, 0x01FE, 0x01F4, 0x81F1,
        0x81D3, 0x01D6, 0x01DC, 0x81D9, 0x01C8, 0x81CD, 0x81C7, 0x01C2,
        0x0140, 0x8145, 0x814F, 0x014A, 0x815B, 0x015E, 0x0154, 0x8151,
        0x8173, 0x0176, 0x017C, 0x8179, 0x0168, 0x816D, 0x8167, 0x0162,
        0x8123, 0x0126, 0x012C, 0x8129, 0x0138, 0x813D, 0x8137, 0x0132,
        0x0110, 0x8115, 0x811F, 0x011A, 0x810B, 0x010E, 0x0104, 0x8101,
        0x8303, 0x0306, 0x030C, 0x8309, 0x0318, 0x831D, 0x8317, 0x0312,
        0x0330, 0x8335, 0x833F, 0x033A, 0x832B, 0x032E, 0x0324, 0x8321,
        0x0360, 0x8365, 0x836F, 0x036A, 0x837B, 0x037E, 0x0374, 0x8371,
        0x8353, 0x0356, 0x035C, 0x8359, 0x0348, 0x834D, 0x8347, 0x0342,
        0x03C0, 0x83C5, 0x83CF, 0x03CA, 0x83DB, 0x03DE, 0x03D4, 0x83D1,
        0x83F3, 0x03F6, 0x03FC, 0x83F9, 0x03E8, 0x83ED, 0x83E7, 0x03E2,
        0x83A3, 0x03A6, 0x03AC, 0x83A9, 0x03B8, 0x83BD, 0x83B7, 0x03B2,
        0x0390, 0x8395, 0x839F, 0x039A, 0x838B, 0x038E, 0x0384, 0x8381,
        0x0280, 0x8285, 0x828F, 0x028A, 0x829B, 0x029E, 0x0294, 0x8291,
        0x82B3, 0x02B6, 0x02BC, 0x82B9, 0x02A8, 0x82AD, 0x82A7, 0x02A2,
        0x82E3, 0x02E6, 0x02EC, 0x82E9, 0x02F8, 0x82FD, 0x82F7, 0x02F2,
        0x02D0, 0x82D5, 0x82DF, 0x02DA, 0x82CB, 0x02CE, 0x02C4, 0x82C1,
        0x8243, 0x0246, 0x024C, 0x8249, 0x0258, 0x825D, 0x8257, 0x0252,
        0x0270, 0x8275, 0x827F, 0x027A, 0x826B, 0x026E, 0x0264, 0x8261,
        0x0220, 0x8225, 0x822F, 0x022A, 0x823B, 0x023E, 0x0234, 0x8231,
        0x8213, 0x0216, 0x021C, 0x8219, 0x0208, 0x820D, 0x8207, 0x0202
    };

    for(j = 0; j < data_blk_size-2; j++)
    {
      i = ((uint16_t)(crc_accum >> 8) ^ data_blk_ptr[j]) & 0xFF;
      crc_accum = (crc_accum << 8) ^ crc_table[i];
    }

    return crc_accum;
}

void DoInst(uint8_t inst){
	switch (inst){
	case INST_INIT_TRIGGER:
		pwm_status.current_TriggerCount = 0;
		pwm_status.total_TriggerCount = 1;
		pwmStart(_DEF_PWM1);
		break;
	case INST_SET_TRIGGER_DELAY:
		pwm_status.delay = pwm_packet.param;
		break;
	case INST_SET_TRIGGER_COUNT:
		pwm_status.total_TriggerCount = pwm_packet.param;
		break;
	case INST_START_TRIGGER:
		pwm_status.current_TriggerCount=0;
		pwmStart(_DEF_PWM1);
		pwmSet_SycDelay(_DEF_PWM1, pwm_status.delay);
		pwmStart(_DEF_PWM2);
		break;
	}
}

bool pwmInit(void)
{
  bool ret =true;

  for (int i=0 ; i<PWM_MAX_CH; i++)
    {
      pwm_tbl[i].is_timeInit=false;
      pwm_tbl[i].is_pwmInit=false;

      pwmBegin(i, pwm_tbl[i].period, pwm_tbl[i].pulse,  pwm_tbl[i].prescaler);
    }

#ifdef _USE_HW_CLI
 cliAdd("PWM", cliPWM);
  #endif

  return ret;
}
bool pwmBegin(uint8_t ch, uint32_t period, uint32_t pulse, uint32_t prescaler)
{
  bool ret = false;

  if (ch >= PWM_MAX_CH) return false;

  pwm_t *p_pwm = &pwm_tbl[ch];
  p_pwm->period = period-1;
  p_pwm->pulse = pulse-1;
  p_pwm->prescaler = prescaler-1;

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  switch(ch)
  {

  case _DEF_PWM1:
      p_pwm->h_tim=&htim2;
      p_pwm->h_tim->Instance = TIM2;
      p_pwm->h_tim->Init.Prescaler = p_pwm->prescaler;
      p_pwm->h_tim->Init.CounterMode = TIM_COUNTERMODE_UP;
      p_pwm->h_tim->Init.Period = p_pwm->period;
      p_pwm->h_tim->Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
      p_pwm->h_tim->Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
        if (HAL_TIM_Base_Init(p_pwm->h_tim) == HAL_OK)
        {
          p_pwm->is_timeInit = true;
          ret= true;
        }
        sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
        if (HAL_TIM_ConfigClockSource(p_pwm->h_tim, &sClockSourceConfig) != HAL_OK)
        {
          Error_Handler();
        }

        if (HAL_TIM_PWM_Init(p_pwm->h_tim) != HAL_OK)
        {
          Error_Handler();
        }

        sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
        sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
        if (HAL_TIMEx_MasterConfigSynchronization(p_pwm->h_tim, &sMasterConfig) != HAL_OK)
        {
          Error_Handler();
        }

        sConfigOC.OCMode = TIM_OCMODE_PWM1;
        sConfigOC.Pulse = p_pwm->pulse;
        sConfigOC.OCPolarity = TIM_OCPOLARITY_LOW;
        sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
        if (HAL_TIM_PWM_ConfigChannel(p_pwm->h_tim, &sConfigOC, p_pwm->channel) == HAL_OK)
        {
            p_pwm->is_pwmInit=true;
        }

        HAL_TIM_MspPostInit(p_pwm->h_tim);
  	break;

    case _DEF_PWM2:
        p_pwm->h_tim=&htim5;
        p_pwm->h_tim->Instance = TIM5;
        p_pwm->h_tim->Init.Prescaler = p_pwm->prescaler;
        p_pwm->h_tim->Init.CounterMode = TIM_COUNTERMODE_UP;
        p_pwm->h_tim->Init.Period = p_pwm->period;
        p_pwm->h_tim->Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
        p_pwm->h_tim->Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
          if (HAL_TIM_Base_Init(p_pwm->h_tim) == HAL_OK)
          {
            p_pwm->is_timeInit = true;
            ret= true;
          }
          sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
          if (HAL_TIM_ConfigClockSource(p_pwm->h_tim, &sClockSourceConfig) != HAL_OK)
          {
            Error_Handler();
          }
          if (HAL_TIM_PWM_Init(p_pwm->h_tim) != HAL_OK)
          {
            Error_Handler();
          }

          sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
          sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
          if (HAL_TIMEx_MasterConfigSynchronization(p_pwm->h_tim, &sMasterConfig) != HAL_OK)
          {
            Error_Handler();
          }

          sConfigOC.OCMode = TIM_OCMODE_PWM1;
          sConfigOC.Pulse = p_pwm->pulse;
          sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
          sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
          if (HAL_TIM_PWM_ConfigChannel(p_pwm->h_tim, &sConfigOC, p_pwm->channel) == HAL_OK)
          {
              p_pwm->is_pwmInit=true;
          }

          HAL_TIM_MspPostInit(p_pwm->h_tim);
    	break;

    case _DEF_PWM3:
      p_pwm->h_tim=&htim5;
      p_pwm->h_tim->Instance = TIM5;
      p_pwm->h_tim->Init.Prescaler = p_pwm->prescaler;
      p_pwm->h_tim->Init.CounterMode = TIM_COUNTERMODE_UP;
      p_pwm->h_tim->Init.Period = p_pwm->period;
      p_pwm->h_tim->Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
      p_pwm->h_tim->Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
        if (HAL_TIM_Base_Init(p_pwm->h_tim) == HAL_OK)
        {
          p_pwm->is_timeInit = true;
          ret= true;
        }

        sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
        if (HAL_TIM_ConfigClockSource(p_pwm->h_tim, &sClockSourceConfig) != HAL_OK)
        {
          Error_Handler();
        }
        if (HAL_TIM_PWM_Init(p_pwm->h_tim) != HAL_OK)
        {
          Error_Handler();
        }

        sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
        sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
        if (HAL_TIMEx_MasterConfigSynchronization(p_pwm->h_tim, &sMasterConfig) != HAL_OK)
        {
          Error_Handler();
        }

        sConfigOC.OCMode = TIM_OCMODE_PWM1;
        sConfigOC.Pulse = p_pwm->pulse;
        sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
        sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
        if (HAL_TIM_PWM_ConfigChannel(p_pwm->h_tim, &sConfigOC, p_pwm->channel) == HAL_OK)
        {
            p_pwm->is_pwmInit=true;
        }
        HAL_TIM_MspPostInit(p_pwm->h_tim);
      break;

  }
  ret = true;
  return ret;
}

void pwmStart(uint8_t ch)
{
  pwm_t* p_pwm = &pwm_tbl[ch];
  HAL_TIM_PWM_Start_IT(p_pwm->h_tim, p_pwm->channel);
}

bool pwmDeinit(uint8_t ch)
{
  bool ret = false;
  if (ch >= PWM_MAX_CH) return ret;

  pwm_t* p_pwm = &pwm_tbl[ch];

  if (HAL_TIM_PWM_DeInit(p_pwm->h_tim)== HAL_OK)
      {
        p_pwm->is_pwmInit=false;
      }
  if (HAL_TIM_Base_DeInit(p_pwm->h_tim)==HAL_OK)
      {
        p_pwm->is_timeInit=false;
      }

  return true;
}

bool pwmStop(uint8_t ch)
{
  if (ch >= PWM_MAX_CH) return false;
  pwm_t* p_pwm = &pwm_tbl[ch];

  HAL_TIM_PWM_Stop_IT(p_pwm->h_tim, p_pwm->channel);

  return true;
}

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance==TIM2)
	{
		pwm_status.current_TriggerCount++;
		if (pwm_status.total_TriggerCount == pwm_status.current_TriggerCount){
			pwmStop(_DEF_PWM1);
			pwm_status.current_TriggerCount=0;
		}
	}
}

//void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
//	if(htim->Instance==TIM2)
//		  {
//				ledToggle(_DEF_LED1);
//		  }
//}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* tim_baseHandle)
{

	  if(tim_baseHandle->Instance==TIM2)
	  {
	    __HAL_RCC_TIM2_CLK_ENABLE();

	    /* TIM2 interrupt Init */
	    HAL_NVIC_SetPriority(TIM2_IRQn, 0, 0);
	    HAL_NVIC_EnableIRQ(TIM2_IRQn);
	  }
	  else if(tim_baseHandle->Instance==TIM5)
  {
    __HAL_RCC_TIM5_CLK_ENABLE();
  }
}

void HAL_TIM_MspPostInit(TIM_HandleTypeDef* timHandle)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  if(timHandle->Instance==TIM2)
  {
    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**TIM2 GPIO Configuration
    PA15 (JTDI)     ------> TIM2_CH1
    */
    GPIO_InitStruct.Pin = GPIO_PIN_15;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);


  }
  else if(timHandle->Instance==TIM5)
  {
	    __HAL_RCC_GPIOA_CLK_ENABLE();
	    __HAL_RCC_GPIOH_CLK_ENABLE();
	    /**TIM5 GPIO Configuration
	    PA1     ------> TIM5_CH2
	    PH10     ------> TIM5_CH1
	    */
	    GPIO_InitStruct.Pin = GPIO_PIN_1;
	    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	    GPIO_InitStruct.Pull = GPIO_NOPULL;
	    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	    GPIO_InitStruct.Alternate = GPIO_AF2_TIM5;
	    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	    GPIO_InitStruct.Pin = GPIO_PIN_10;
	    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	    GPIO_InitStruct.Pull = GPIO_NOPULL;
	    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	    GPIO_InitStruct.Alternate = GPIO_AF2_TIM5;
	    HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);
  }
}
void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* tim_baseHandle)
{

	  if(tim_baseHandle->Instance==TIM2)
	  {
	    __HAL_RCC_TIM2_CLK_DISABLE();
	    HAL_NVIC_DisableIRQ(TIM2_IRQn);

	  }
	  else if(tim_baseHandle->Instance==TIM5)
  {
    __HAL_RCC_TIM5_CLK_DISABLE();
  }
}


#ifdef _USE_HW_CLI

static void cliPWM(cli_args_t *args){
	  bool ret = false;

	  if (args->argc==1 && args->isStr(0, "start")==true)
	    {
		  pwmStart(_DEF_PWM1);
		  pwmStart(_DEF_PWM2);

		  cliPrintf("pwmStart");

	      ret = true;
		}

	  if (args->argc==1 && args->isStr(0, "stop")==true)
	    {
		  pwmStop(_DEF_PWM1);
		  pwmStop(_DEF_PWM2);
		  cliPrintf("pwmStop");
	      ret = true;
	    }

	  uint32_t period;
      period=args->getData(1);

	  if (args->argc==2 && args->isStr(0, "set")==true){
		  pwmStop(_DEF_PWM1);
		  pwmStop(_DEF_PWM2);
	      pwmBegin(_DEF_PWM1, period, pwm_tbl[_DEF_PWM1].pulse, pwm_tbl[_DEF_PWM1].prescaler);
	      pwmBegin(_DEF_PWM2, period, pwm_tbl[_DEF_PWM2].pulse, pwm_tbl[_DEF_PWM2].prescaler);
		  pwmStart(_DEF_PWM1);
		  pwmStart(_DEF_PWM2);
	      cliPrintf("set");
	  }

	  if(ret!=true)
	    {
	      cliPrintf("start\n");
	      cliPrintf("stop\n");
	      cliPrintf("set period\n");

	    }
	}


#endif

#endif

