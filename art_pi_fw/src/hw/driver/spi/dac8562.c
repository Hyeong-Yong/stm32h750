/*
 * dac8562.c
 *
 *  Created on: 2023. 12. 9.
 *      Author: User
 */


#include "SPI.h"
#include "DAC8562.h"
#include "GPIO.h"
#include "cli.h"

static const uint8_t spi_ch = _DEF_SPI1;
static uint16_t vref;


#ifdef _USE_HW_CLI
static void cliDAC8562(cli_args_t *args);
#endif

//DAC8562( uint8_t cs_pin )
//{
//  _cs_pin = cs_pin;
//  _vref=5;
//};

void dac8562_init(){
	  vref = 5;
	  dac8562_initialize();
	  gpioPinWrite(_PIN_GPIO_DAC8562_SYNC, _DEF_RESET);
	  /* !Chip select (low to enable) */
	  //pinMode(_cs_pin, OUTPUT);
	  //digitalWrite(_cs_pin,  1);


#ifdef _USE_HW_CLI
	  cliAdd("dac8562", cliDAC8562);
#endif

}


void dac8562_writeReg(uint8_t cmd_byte, uint16_t data_byte) {
  gpioPinWrite(_PIN_GPIO_DAC8562_SYNC, _DEF_SET);
  spiTransfer8(spi_ch, cmd_byte);
  spiTransfer16(spi_ch, cmd_byte);
  //SPI.transfer(cmd_byte);
  //SPI.transfer16(data_byte);
  gpioPinWrite(_PIN_GPIO_DAC8562_SYNC, _DEF_RESET);
};


void dac8562_outputValue(uint8_t cmd_byte, uint16_t input) {
  uint8_t inputMid = (input>>8)&0xFF;
  uint8_t inputLast = input&0xFF;
  //uint16_t t= (input>>8) & 0xFF;
  dac8562_writeValue(cmd_byte, (inputLast),(inputMid));
};

void dac8562_writeVoltage(float input) {
  writeA(input);
  writeB(input);
};

void writeA(float input) {
 dac8562_outputValue(CMD_SETA_UPDATEA,dac8562_voltageConvert(input/vref*5));
};

void writeB(float input) {
	dac8562_outputValue(CMD_SETB_UPDATEB,dac8562_voltageConvert(input/vref*5));
};

void dac8562_writeValue(uint8_t cmd_byte, uint8_t mid, uint8_t last) {
	gpioPinWrite(_PIN_GPIO_DAC8562_SYNC, _DEF_SET);
	spiTransfer8(spi_ch, cmd_byte);
	spiTransfer8(spi_ch, last);
	spiTransfer8(spi_ch, mid);
//  SPI.transfer(cmd_byte);
//  SPI.transfer(last);
//  SPI.transfer(mid);
	gpioPinWrite(_PIN_GPIO_DAC8562_SYNC, _DEF_RESET);
};

void dac8562_initialize() {
	dac8562_writeReg(CMD_RESET_ALL_REG, DATA_RESET_ALL_REG);      // reset
	dac8562_writeReg(CMD_PWR_UP_A_B, DATA_PWR_UP_A_B);        // power up
	dac8562_writeReg(CMD_INTERNAL_REF_EN, DATA_INTERNAL_REF_EN);      // enable internal reference
	dac8562_writeReg(CMD_GAIN, DATA_GAIN_B2_A2);            // set multiplier
	dac8562_writeReg(CMD_LDAC_DIS, DATA_LDAC_DIS);          // update the caches
};

uint16_t dac8562_voltageConvert(float voltage)
{
  uint16_t _D_;

  voltage = voltage / 6  + 2.5;   //based on the manual provided by texas instruments

  _D_ = (uint16_t)(65536 * voltage / 5);

  if(_D_ < 32768)
  {
    _D_ -= 100;     //fix the errors
  }

  return _D_;
};


void cliDAC8562(cli_args_t *args){

}

