/*
 * timer.c
 *
 *  Created on: 2024. 4. 25.
 *      Author: User
 */


#include "cli.h"
#include "pwm.h"
#include "led.h"

#ifdef _USE_HW_PWM

#ifdef _USE_HW_CLI
static void cliPWM(cli_args_t *args);

#endif

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

TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim5;


static pwm_t pwm_tbl[PWM_MAX_CH] =
    {
     {&htim2, TIM_CHANNEL_1, 1000000, 240,   10}, //PA15, RF GENERATOR TRIGGER IN (Windfreak)
     {&htim5, TIM_CHANNEL_1, 1000000, 240,   10}, //PH10, DIGITIZER TRIGGER IN (AlazarTech)
	 // 1000000 = 1 sec, 1000 = 1ms, 1 = 1 us
 	 {&htim5, TIM_CHANNEL_2, 1000000, 240,   10000}, //PA1,
    };



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

void pwmSycDelay(uint8_t ch, uint32_t delay)
{
  pwm_t* p_pwm = &pwm_tbl[ch];
  // 1 clock is delayed when using encapulated function. To synchronize, add 1 clock
  p_pwm->h_tim->Instance->CNT = delay+1;
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

				ledToggle(_DEF_LED1);
		  }
}


//
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

