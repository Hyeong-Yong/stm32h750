/*
 * spi.c
 *
 *  Created on: 2023. 7. 5.
 *      Author: User
 */


#include "spi.h"



typedef struct
{
  bool is_open;
  bool is_tx_done;
  bool is_rx_done;
  void (*func)(void);

  SPI_HandleTypeDef *h_spi;
  DMA_HandleTypeDef *h_dma;
} spi_t;



spi_t spi_tbl[SPI_MAX_CH];

SPI_HandleTypeDef hspi4; // DAC8562
SPI_HandleTypeDef hspi2; // DAC8562

//DMA_HandleTypeDef hdma_spi4_tx; //DAC8562


bool spiInit(void)
{
  bool ret = true;


  for (int i=0; i<SPI_MAX_CH; i++)
  {
    spi_tbl[i].is_open = false;
    spi_tbl[i].is_tx_done = true;
    spi_tbl[i].func = NULL;
  }

  return ret;
}

bool spiOpen(uint8_t ch)
{
  bool ret = false;
  spi_t *p_spi = &spi_tbl[ch];

  switch(ch)
  {
    case _DEF_SPI1:
    	  p_spi->h_spi = &hspi4;
    	  p_spi->h_spi->Instance=SPI4;

    	  p_spi->h_spi->Init.Mode = SPI_MODE_MASTER;
    	  p_spi->h_spi->Init.Direction = SPI_DIRECTION_2LINES_TXONLY;
    	  p_spi->h_spi->Init.DataSize = SPI_DATASIZE_8BIT;
    	  p_spi->h_spi->Init.CLKPolarity = SPI_POLARITY_HIGH;
    	  p_spi->h_spi->Init.CLKPhase = SPI_PHASE_2EDGE;
    	  p_spi->h_spi->Init.NSS = SPI_NSS_SOFT;
    	  p_spi->h_spi->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
    	  p_spi->h_spi->Init.FirstBit = SPI_FIRSTBIT_MSB;
    	  p_spi->h_spi->Init.TIMode = SPI_TIMODE_DISABLE;
    	  p_spi->h_spi->Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    	  p_spi->h_spi->Init.CRCPolynomial = 0x0;
    	  p_spi->h_spi->Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
    	  p_spi->h_spi->Init.NSSPolarity = SPI_NSS_POLARITY_LOW;
    	  p_spi->h_spi->Init.FifoThreshold = SPI_FIFO_THRESHOLD_01DATA;
    	  p_spi->h_spi->Init.TxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
    	  p_spi->h_spi->Init.RxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
    	  p_spi->h_spi->Init.MasterSSIdleness = SPI_MASTER_SS_IDLENESS_00CYCLE;
    	  p_spi->h_spi->Init.MasterInterDataIdleness = SPI_MASTER_INTERDATA_IDLENESS_00CYCLE;
    	  p_spi->h_spi->Init.MasterReceiverAutoSusp = SPI_MASTER_RX_AUTOSUSP_DISABLE;
    	  p_spi->h_spi->Init.MasterKeepIOState = SPI_MASTER_KEEP_IO_STATE_DISABLE;
    	  p_spi->h_spi->Init.IOSwap = SPI_IO_SWAP_DISABLE;

    	  if (HAL_SPI_Init(p_spi->h_spi) == HAL_OK)
    	  {
    		  p_spi->is_open = true;
    		  ret = true;
    	  }
    	break;
    case _DEF_SPI2:
    	  hspi2.Instance = SPI2;
    	  hspi2.Init.Mode = SPI_MODE_MASTER;
    	  hspi2.Init.Direction = SPI_DIRECTION_2LINES_TXONLY;
    	  hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
    	  hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
    	  hspi2.Init.CLKPhase = SPI_PHASE_2EDGE;
    	  hspi2.Init.NSS = SPI_NSS_SOFT;
    	  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
    	  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
    	  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
    	  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    	  hspi2.Init.CRCPolynomial = 0x0;
    	  hspi2.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
    	  hspi2.Init.NSSPolarity = SPI_NSS_POLARITY_LOW;
    	  hspi2.Init.FifoThreshold = SPI_FIFO_THRESHOLD_01DATA;
    	  hspi2.Init.TxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
    	  hspi2.Init.RxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
    	  hspi2.Init.MasterSSIdleness = SPI_MASTER_SS_IDLENESS_00CYCLE;
    	  hspi2.Init.MasterInterDataIdleness = SPI_MASTER_INTERDATA_IDLENESS_00CYCLE;
    	  hspi2.Init.MasterReceiverAutoSusp = SPI_MASTER_RX_AUTOSUSP_DISABLE;
    	  hspi2.Init.MasterKeepIOState = SPI_MASTER_KEEP_IO_STATE_DISABLE;
    	  hspi2.Init.IOSwap = SPI_IO_SWAP_DISABLE;
    	  if (HAL_SPI_Init(&hspi2) != HAL_OK)
    	  {
    	    Error_Handler();
    	  }
    	break;
  }

  return ret;
}

void spiSetDataMode(uint8_t ch, uint8_t dataMode)
{
  spi_t  *p_spi = &spi_tbl[ch];


  if (p_spi->is_open == false) return;


  switch( dataMode )
  {
    // CPOL=0, CPHA=0
    case SPI_MODE0:
      p_spi->h_spi->Init.CLKPolarity = SPI_POLARITY_LOW;
      p_spi->h_spi->Init.CLKPhase    = SPI_PHASE_1EDGE;
      HAL_SPI_Init(p_spi->h_spi);
      break;

    // CPOL=0, CPHA=1
    case SPI_MODE1:
      p_spi->h_spi->Init.CLKPolarity = SPI_POLARITY_LOW;
      p_spi->h_spi->Init.CLKPhase    = SPI_PHASE_2EDGE;
      HAL_SPI_Init(p_spi->h_spi);
      break;

    // CPOL=1, CPHA=0
    case SPI_MODE2:
      p_spi->h_spi->Init.CLKPolarity = SPI_POLARITY_HIGH;
      p_spi->h_spi->Init.CLKPhase    = SPI_PHASE_1EDGE;
      HAL_SPI_Init(p_spi->h_spi);
      break;

    // CPOL=1, CPHA=1
    case SPI_MODE3:
      p_spi->h_spi->Init.CLKPolarity = SPI_POLARITY_HIGH;
      p_spi->h_spi->Init.CLKPhase    = SPI_PHASE_2EDGE;
      HAL_SPI_Init(p_spi->h_spi);
      break;
  }
}

void spiSetBitWidth(uint8_t ch, uint8_t bit_width)
{
  spi_t  *p_spi = &spi_tbl[ch];

  if (p_spi->is_open == false) return;

  p_spi->h_spi->Init.DataSize = SPI_DATASIZE_8BIT;

  if (bit_width == 16)
  {
    p_spi->h_spi->Init.DataSize = SPI_DATASIZE_16BIT;
  }
  HAL_SPI_Init(p_spi->h_spi);
}

uint8_t spiTransfer8(uint8_t ch, uint8_t data)
{
  uint8_t ret;
  spi_t  *p_spi = &spi_tbl[ch];


  if (p_spi->is_open == false) return 0;

  HAL_SPI_TransmitReceive(p_spi->h_spi, &data, &ret, 1, 100);

  return ret;
}

uint16_t spiTransfer16(uint8_t ch, uint16_t data)
{
  uint8_t tBuf[2];
  uint8_t rBuf[2];
  uint16_t ret;
  spi_t  *p_spi = &spi_tbl[ch];


  if (p_spi->is_open == false) return 0;

  if (p_spi->h_spi->Init.DataSize == SPI_DATASIZE_8BIT)
  {
    tBuf[1] = (uint8_t)data;
    tBuf[0] = (uint8_t)(data>>8);
    HAL_SPI_TransmitReceive(p_spi->h_spi, (uint8_t *)&tBuf, (uint8_t *)&rBuf, 2, 100);

    ret = rBuf[0];
    ret <<= 8;
    ret += rBuf[1];
  }
  else
  {
    HAL_SPI_TransmitReceive(p_spi->h_spi, (uint8_t *)&data, (uint8_t *)&ret, 1, 100);
  }

  return ret;
}


void spiTx(uint8_t spi_ch, uint8_t *p_buf, uint32_t length)
{
  spi_t  *p_spi = &spi_tbl[spi_ch];

  if (p_spi->is_open == false) return;

  HAL_SPI_Transmit(p_spi->h_spi, p_buf, length, 100);
}




void spiDmaTxStart(uint8_t spi_ch, uint8_t *p_buf, uint32_t length)
{
  spi_t  *p_spi = &spi_tbl[spi_ch];

  if (p_spi->is_open == false) return;

  p_spi->is_tx_done = false;
  HAL_SPI_Transmit_DMA(p_spi->h_spi, p_buf, length);
}

void spiDmaRxStart(uint8_t spi_ch, uint8_t *p_buf, uint32_t length)
{
  spi_t  *p_spi = &spi_tbl[spi_ch];

  if (p_spi->is_open == false) return;

  p_spi->is_rx_done = false;
  HAL_SPI_Receive_DMA(p_spi->h_spi, p_buf, length);
}

void spiDmaTxTransfer(uint8_t ch, void *buf, uint32_t length, uint32_t timeout)
{
  uint32_t t_time;


  spiDmaTxStart(ch, (uint8_t *)buf, length);

  t_time = millis();

  if (timeout == 0) return;

  while(1)
  {
    if(spiDmaTxIsDone(ch))
    {
      break;
    }
    if((millis()-t_time) > timeout)
    {
      break;
    }
  }
}



bool spiDmaTxIsDone(uint8_t ch)
{
  spi_t  *p_spi = &spi_tbl[ch];

  if (p_spi->is_open == false)     return true;

  return p_spi->is_tx_done;
}

bool spiDmaRxIsDone(uint8_t ch)
{
  spi_t  *p_spi = &spi_tbl[ch];

  if (p_spi->is_open == false)     return true;

  return p_spi->is_rx_done;
}


void spiAttachTxInterrupt(uint8_t ch, void (*func)())
{
  spi_t  *p_spi = &spi_tbl[ch];


  if (p_spi->is_open == false)     return;

  p_spi->func = func;
}


void spiAttachRxInterrupt(uint8_t ch, void (*func)())
{
  spi_t  *p_spi = &spi_tbl[ch];


  if (p_spi->is_open == false)     return;

  p_spi->func = func;
}


void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
  spi_t  *p_spi;

  if (hspi->Instance == spi_tbl[_DEF_SPI1].h_spi->Instance)
  {
    p_spi = &spi_tbl[_DEF_SPI1];

    p_spi->is_tx_done = true;

    if (p_spi->func != NULL)
    {
      (*p_spi->func)();
    }
  }
}


void HAL_SPI_MspInit(SPI_HandleTypeDef* spiHandle)
{
	  GPIO_InitTypeDef GPIO_InitStruct = {0};
	  if(spiHandle->Instance==SPI2)
	  {
	  /* USER CODE BEGIN SPI2_MspInit 0 */

	  /* USER CODE END SPI2_MspInit 0 */
	    /* SPI2 clock enable */
	    __HAL_RCC_SPI2_CLK_ENABLE();

	    __HAL_RCC_GPIOI_CLK_ENABLE();
	    /**SPI2 GPIO Configuration
	    PI1     ------> SPI2_SCK
	    PI3     ------> SPI2_MOSI
	    */
	    GPIO_InitStruct.Pin = GPIO_PIN_1|GPIO_PIN_3;
	    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	    GPIO_InitStruct.Pull = GPIO_NOPULL;
	    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	    GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
	    HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);

	  /* USER CODE BEGIN SPI2_MspInit 1 */

	  /* USER CODE END SPI2_MspInit 1 */
	  }
	  else if(spiHandle->Instance==SPI4)
	  {
	  /* USER CODE BEGIN SPI4_MspInit 0 */

	  /* USER CODE END SPI4_MspInit 0 */
	    /* SPI4 clock enable */
	    __HAL_RCC_SPI4_CLK_ENABLE();

	    __HAL_RCC_GPIOE_CLK_ENABLE();
	    /**SPI4 GPIO Configuration
	    PE2     ------> SPI4_SCK
	    PE6     ------> SPI4_MOSI
	    */
	    GPIO_InitStruct.Pin = GPIO_PIN_2;
	    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	    GPIO_InitStruct.Pull = GPIO_NOPULL;
	    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	    GPIO_InitStruct.Alternate = GPIO_AF5_SPI4;
	    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

	    GPIO_InitStruct.Pin = GPIO_PIN_6;
	    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	    GPIO_InitStruct.Pull = GPIO_NOPULL;
	    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	    GPIO_InitStruct.Alternate = GPIO_AF5_SPI4;
	    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

	  /* USER CODE BEGIN SPI4_MspInit 1 */

	  /* USER CODE END SPI4_MspInit 1 */
	  }
}

void HAL_SPI_MspDeInit(SPI_HandleTypeDef* spiHandle)
{

	  if(spiHandle->Instance==SPI2)
	  {
	  /* USER CODE BEGIN SPI2_MspDeInit 0 */

	  /* USER CODE END SPI2_MspDeInit 0 */
	    /* Peripheral clock disable */
	    __HAL_RCC_SPI2_CLK_DISABLE();

	    /**SPI2 GPIO Configuration
	    PI1     ------> SPI2_SCK
	    PI3     ------> SPI2_MOSI
	    */
	    HAL_GPIO_DeInit(GPIOI, GPIO_PIN_1|GPIO_PIN_3);

	  /* USER CODE BEGIN SPI2_MspDeInit 1 */

	  /* USER CODE END SPI2_MspDeInit 1 */
	  }
	  else if(spiHandle->Instance==SPI4)
	  {
	  /* USER CODE BEGIN SPI4_MspDeInit 0 */

	  /* USER CODE END SPI4_MspDeInit 0 */
	    /* Peripheral clock disable */
	    __HAL_RCC_SPI4_CLK_DISABLE();

	    /**SPI4 GPIO Configuration
	    PE2     ------> SPI4_SCK
	    PE6     ------> SPI4_MOSI
	    */
	    HAL_GPIO_DeInit(GPIOE, GPIO_PIN_2|GPIO_PIN_6);

	  /* USER CODE BEGIN SPI4_MspDeInit 1 */

	  /* USER CODE END SPI4_MspDeInit 1 */
	  }
}
