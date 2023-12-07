/*
 * adc.c
 *
 *  Created on: 2023. 6. 10.
 *      Author: hwang
 */

#include "adc.h"
#include "uart.h"

#ifdef _USE_HW_ADC


ADC_HandleTypeDef hadc1;
ADC_HandleTypeDef hadc2;
DMA_HandleTypeDef hdma_adc1;



#ifdef _USE_HW_CLI
static void cliADC(cli_args_t *args);
#endif


/* ADC parameters */
#define ADC_BUF_SIZE ((uint32_t)  8)    /* Size of array containing ADC converted values */

/* Variable containing ADC conversions results */
static __attribute__((section(".non_cache"))) ALIGN_32BYTES(__IO uint32_t adcBuf[ADC_BUF_SIZE]);    /* ADC dual mode interleaved conversion results (ADC master and ADC slave results concatenated on data register 32 bits of ADC master). */





typedef struct
{
	ADC_HandleTypeDef 		  *hadc_master;
	ADC_HandleTypeDef 		  *hadc_slave;
	DMA_HandleTypeDef 		  *hadc_DMA;
    __IO bool			      adc_transfer_done;
	bool					  is_open_interleaved;
} adc_interleaved_tbl_t;


adc_interleaved_tbl_t adc_tbl[ADC_MAX_CH] =
	{
        {&hadc1, &hadc2, &hdma_adc1, false, false},
    };

bool adcInit(){
	bool ret = true;

#ifdef _USE_HW_CLI
  cliAdd("adc", cliADC);
#endif


	return ret;
}


bool adcInitInterleaved(){
	bool ret= true;

	ret &= MX_ADC1_Init();
	ret &= MX_ADC2_Init();

	if (HAL_ADCEx_Calibration_Start(adc_tbl[_DEF_INTERLEAVED].hadc_master, ADC_CALIB_OFFSET_LINEARITY, ADC_SINGLE_ENDED) != HAL_OK){
			ret = false;
		  	cliPrintf("ADC master calibration failed");
		}

	if (HAL_ADCEx_Calibration_Start(adc_tbl[_DEF_INTERLEAVED].hadc_slave, ADC_CALIB_OFFSET_LINEARITY, ADC_SINGLE_ENDED) != HAL_OK){
		ret = false;
  		cliPrintf("ADC slave calibration failed");
	}

		if ( HAL_ADCEx_MultiModeStart_DMA(adc_tbl[_DEF_INTERLEAVED].hadc_master,
										(uint32_t *)adcBuf,
			                               ADC_BUF_SIZE) != HAL_OK)
			{
				ret = false;
			}

		LL_ADC_SetMultiDMATransfer(ADC12_COMMON, LL_ADC_MULTI_REG_DMA_RES_32_10B);
		__HAL_DMA_DISABLE_IT(adc_tbl[_DEF_INTERLEAVED].hadc_DMA, DMA_IT_HT);

	adc_tbl[_DEF_INTERLEAVED].is_open_interleaved = ret;
	adc_tbl[_DEF_INTERLEAVED].adc_transfer_done = true;

	return ret;
}

bool adcIsInitInterleaved(){
	return adc_tbl[_DEF_INTERLEAVED].is_open_interleaved;
}



bool adcSingleMeasure(){
	bool ret = true;
	if ( HAL_ADCEx_MultiModeStart_DMA(adc_tbl[_DEF_INTERLEAVED].hadc_master,
			                          (uint32_t *)adcBuf,
		                               ADC_BUF_SIZE) != HAL_OK)
		{
			ret = false;
		}

	LL_ADC_SetMultiDMATransfer(ADC12_COMMON, LL_ADC_MULTI_REG_DMA_RES_32_10B);
	__HAL_DMA_DISABLE_IT(adc_tbl[_DEF_INTERLEAVED].hadc_DMA, DMA_IT_HT);

	return ret;
}


void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *AdcHandle)
{
	adc_tbl[_DEF_INTERLEAVED].adc_transfer_done = true;
}

void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef* hadc)
{

}



bool MX_ADC1_Init(void)
{
  bool ret = true;
  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_MultiModeTypeDef multimode = {0};
  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Common config
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc1.Init.LowPowerAutoWait = DISABLE;
  hadc1.Init.ContinuousConvMode = ENABLE;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ConversionDataManagement = ADC_CONVERSIONDATA_DMA_ONESHOT;
  hadc1.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  hadc1.Init.LeftBitShift = ADC_LEFTBITSHIFT_NONE;
  hadc1.Init.OversamplingMode = DISABLE;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    ret = false;
  }

  /** Configure the ADC multi-mode
  */
  multimode.Mode = ADC_DUALMODE_INTERL;
  multimode.DualModeData = ADC_DUALMODEDATAFORMAT_32_10_BITS;
  multimode.TwoSamplingDelay = ADC_TWOSAMPLINGDELAY_3CYCLES;
  if (HAL_ADCEx_MultiModeConfigChannel(&hadc1, &multimode) != HAL_OK)
  {
    ret = false;
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_1;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_2CYCLE_5;
  sConfig.SingleDiff = ADC_SINGLE_ENDED;
  sConfig.OffsetNumber = ADC_OFFSET_NONE;
  sConfig.Offset = 0;
  sConfig.OffsetSignedSaturation = DISABLE;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    ret = false;
  }

  return ret;
}

/* ADC2 init function */
bool MX_ADC2_Init(void)
{

  bool ret = true;
  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC2_Init 1 */

  /* USER CODE END ADC2_Init 1 */

  /** Common config
  */
  hadc2.Instance = ADC2;
  hadc2.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
  hadc2.Init.Resolution = ADC_RESOLUTION_12B;
  hadc2.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc2.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc2.Init.LowPowerAutoWait = DISABLE;
  hadc2.Init.ContinuousConvMode = ENABLE;
  hadc2.Init.NbrOfConversion = 1;
  hadc2.Init.DiscontinuousConvMode = DISABLE;
  hadc2.Init.ConversionDataManagement = ADC_CONVERSIONDATA_DMA_ONESHOT;
  hadc2.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  hadc2.Init.LeftBitShift = ADC_LEFTBITSHIFT_NONE;
  hadc2.Init.OversamplingMode = DISABLE;
  if (HAL_ADC_Init(&hadc2) != HAL_OK)
  {
    ret = false;
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_1;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_2CYCLES_5;
  sConfig.SingleDiff = ADC_SINGLE_ENDED;
  sConfig.OffsetNumber = ADC_OFFSET_NONE;
  sConfig.Offset = 0;
  sConfig.OffsetSignedSaturation = DISABLE;
  if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK)
  {
    ret = false;
  }

  return ret;
}




static uint32_t HAL_RCC_ADC12_CLK_ENABLED=0;

void HAL_ADC_MspInit(ADC_HandleTypeDef* adcHandle)
{

  if(adcHandle->Instance==ADC1)
  {

    /* ADC1 clock enable */
    HAL_RCC_ADC12_CLK_ENABLED++;
    if(HAL_RCC_ADC12_CLK_ENABLED==1){
      __HAL_RCC_ADC12_CLK_ENABLE();
    }

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**ADC1 GPIO Configuration
    PA1_C     ------> ADC1_INP1
    */
    HAL_SYSCFG_AnalogSwitchConfig(SYSCFG_SWITCH_PA1, SYSCFG_SWITCH_PA1_OPEN);

    /* DMA controller clock enable */
    __HAL_RCC_DMA1_CLK_ENABLE();

    /* DMA interrupt init */
    /* DMA1_Stream1_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(DMA1_Stream1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(DMA1_Stream1_IRQn);


    /* ADC1 DMA Init */
    /* ADC1 Init */
    hdma_adc1.Instance = DMA1_Stream1;
    hdma_adc1.Init.Request = DMA_REQUEST_ADC1;
    hdma_adc1.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_adc1.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_adc1.Init.MemInc = DMA_MINC_ENABLE;
    hdma_adc1.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
    hdma_adc1.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
    hdma_adc1.Init.Mode = DMA_NORMAL;
    hdma_adc1.Init.Priority = DMA_PRIORITY_HIGH;
    hdma_adc1.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_adc1) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(adcHandle,DMA_Handle,hdma_adc1);



    /* ADC1 interrupt Init */
    //HAL_NVIC_SetPriority(ADC_IRQn, 0, 0);
    //HAL_NVIC_EnableIRQ(ADC_IRQn);
  }
  else if(adcHandle->Instance==ADC2)
  {
    /* ADC2 clock enable */
    HAL_RCC_ADC12_CLK_ENABLED++;
    if(HAL_RCC_ADC12_CLK_ENABLED==1){
      __HAL_RCC_ADC12_CLK_ENABLE();
    }

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**ADC2 GPIO Configuration
    PA1_C     ------> ADC2_INP1
    */
    HAL_SYSCFG_AnalogSwitchConfig(SYSCFG_SWITCH_PA1, SYSCFG_SWITCH_PA1_OPEN);

    /* ADC2 interrupt Init */
    //HAL_NVIC_SetPriority(ADC_IRQn, 0, 0);
    //HAL_NVIC_EnableIRQ(ADC_IRQn);
  }
}

void HAL_ADC_MspDeInit(ADC_HandleTypeDef* adcHandle)
{

  if(adcHandle->Instance==ADC1)
  {
    /* Peripheral clock disable */
    HAL_RCC_ADC12_CLK_ENABLED--;
    if(HAL_RCC_ADC12_CLK_ENABLED==0){
      __HAL_RCC_ADC12_CLK_DISABLE();
    }

    /* ADC1 DMA DeInit */
    HAL_DMA_DeInit(adcHandle->DMA_Handle);

    /* ADC1 interrupt Deinit */
  /* USER CODE BEGIN ADC1:ADC_IRQn disable */
    /**
    * Uncomment the line below to disable the "ADC_IRQn" interrupt
    * Be aware, disabling shared interrupt may affect other IPs
    */
    /* HAL_NVIC_DisableIRQ(ADC_IRQn); */
  /* USER CODE END ADC1:ADC_IRQn disable */

  }
  else if(adcHandle->Instance==ADC2)
  {

    /* Peripheral clock disable */
    HAL_RCC_ADC12_CLK_ENABLED--;
    if(HAL_RCC_ADC12_CLK_ENABLED==0){
      __HAL_RCC_ADC12_CLK_DISABLE();
    }

    /* ADC2 interrupt Deinit */
  /* USER CODE BEGIN ADC2:ADC_IRQn disable */
    /**
    * Uncomment the line below to disable the "ADC_IRQn" interrupt
    * Be aware, disabling shared interrupt may affect other IPs
    */
    /* HAL_NVIC_DisableIRQ(ADC_IRQn); */
  /* USER CODE END ADC2:ADC_IRQn disable */

  }
}


#ifdef _USE_HW_CLI
void cliADC(cli_args_t *args)
{
  bool ret = false;
  __IO uint32_t i;

  if (args->argc == 1 && args->isStr(0, "run") == true)
  {

	  adcInitInterleaved();
			while(cliKeepLoop())
			{
				cliPrintf("-------------------------\n");
				if (adc_tbl[_DEF_INTERLEAVED].adc_transfer_done == true)
				{
					for (i=0;i<ADC_BUF_SIZE; i++)
					{
						cliPrintf( "%u\n", (uint16_t)(adcBuf[i]&0x0000FFFF) ); // LSB-> Master
						cliPrintf("%u\n", (uint16_t)(adcBuf[i]>>16));        // MSB-> SLAVE
					}
					delay(1000);
					adc_tbl[_DEF_INTERLEAVED].adc_transfer_done = false;
					if (adcSingleMeasure() == false)
						{
						 cliPrintf("ADC single measurement failed\n");
						}
				}

			}
    ret = true;
  }

  if (ret != true)
  {
    cliPrintf("ADC run\n");
  }

}

#endif











#endif
