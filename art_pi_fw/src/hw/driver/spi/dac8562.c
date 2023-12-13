/*
 * dac8562.c
 *
 *  Created on: 2023. 12. 9.
 *      Author: User
 */


#include "SPI.h"
#include "DAC8562.h"
#include "cli.h"
#include "GPIO.h"

typedef struct DAC8562{
	uint16_t vref;
	uint8_t cs_pin;
} dac8562_t;

dac8562_t dac8562;

//DAC8562( uint8_t cs_pin )
//{
//  _cs_pin = cs_pin;
//  _vref=5;
//};

void dac8562_init(){

	  dac8562_initialize();
	  gpioPinWrite(_PIN_GPIO_DAC8562_SYNC, GPIO_PIN_SET);
	  /* !Chip select (low to enable) */
	  //pinMode(_cs_pin, OUTPUT);
	  //digitalWrite(_cs_pin,  1);
}


void DAC_WR_REG(uint8_t cmd_byte, uint16_t data_byte) {
  digitalWrite(_cs_pin, 0);
  SPI.transfer(cmd_byte);
  SPI.transfer16(data_byte);
  digitalWrite(_cs_pin, 1);
};


void outPutValue(uint8_t cmd_byte, uint16_t input) {
  byte inputMid = (input>>8)&0xFF;
  byte inputLast = input&0xFF;
  unsigned int  t= (input>>8)&0xFF;
  writeValue(cmd_byte, (inputLast),(inputMid));
};

void writeVoltage(float input) {
  writeA(input);
  writeB(input);
};

void writeA(float input) {
 outPutValue(CMD_SETA_UPDATEA,Voltage_Convert(input/_vref*5));
};

void writeB(float input) {
 outPutValue(CMD_SETB_UPDATEB,Voltage_Convert(input/_vref*5));
};

void writeValue(uint8_t cmd_byte, uint8_t mid, uint8_t last) {
  digitalWrite(_cs_pin, 0);
  SPI.transfer(cmd_byte);
  SPI.transfer(last);
  SPI.transfer(mid);
  digitalWrite(_cs_pin, 1);
};

void dac8562_initialize() {
  DAC_WR_REG(CMD_RESET_ALL_REG, DATA_RESET_ALL_REG);      // reset
  DAC_WR_REG(CMD_PWR_UP_A_B, DATA_PWR_UP_A_B);        // power up
  DAC_WR_REG(CMD_INTERNAL_REF_EN, DATA_INTERNAL_REF_EN);      // enable internal reference
  DAC_WR_REG(CMD_GAIN, DATA_GAIN_B2_A2);            // set multiplier
  DAC_WR_REG(CMD_LDAC_DIS, DATA_LDAC_DIS);          // update the caches
};

uint16_t Voltage_Convert(float voltage)
{
  u16 _D_;

  voltage = voltage / 6  + 2.5;   //based on the manual provided by texas instruments

  _D_ = (u16)(65536 * voltage / 5);

  if(_D_ < 32768)
  {
    _D_ -= 100;     //fix the errors
  }

  return _D_;
};

