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
static float vref;
float voltage_A;
float voltage_B;

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
  spiTransfer16(spi_ch, data_byte);
  //SPI.transfer(cmd_byte);
  //SPI.transfer16(data_byte);
  gpioPinWrite(_PIN_GPIO_DAC8562_SYNC, _DEF_RESET);
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

void dac8562_outputValue(uint8_t cmd_byte, uint16_t data_value) {
	dac8562_writeReg(cmd_byte, data_value);
};

//void dac8562_outputValue(uint8_t cmd_byte, uint16_t input) {
//  uint8_t inputMid = (input>>8)&0xFF;
//  uint8_t inputLast = input&0xFF;
//  //uint16_t t= (input>>8) & 0xFF;
//  dac8562_writeValue(cmd_byte, (inputLast),(inputMid));
//};


void dac8562_writeVoltage_AB(float voltage) {
	dac8562_writeVoltage_A(voltage);
	dac8562_writeVoltage_B(voltage);
};

void dac8562_writeVoltage_A(float voltage) {
	dac8562_outputValue(CMD_SETA_UPDATEA, dac8562_voltageConvert(voltage));
	voltage_A = voltage;
};

void dac8562_writeVoltage_B(float voltage) {
	dac8562_outputValue(CMD_SETB_UPDATEB,dac8562_voltageConvert(voltage));
	voltage_B = voltage;
};

void dac8562_initialize() {
	dac8562_writeReg(CMD_RESET_ALL_REG, DATA_RESET_ALL_REG);      // reset
	dac8562_writeReg(CMD_PWR_UP_A_B, DATA_PWR_UP_A_B);    	      // power up
	dac8562_writeReg(CMD_INTERNAL_REF_EN, DATA_INTERNAL_REF_EN);  // enable internal reference
	dac8562_writeReg(CMD_GAIN, DATA_GAIN_A1_B1);            	  // set multiplier
	dac8562_writeReg(CMD_LDAC_DIS, DATA_LDAC_DIS);          	  // update the caches

	//중간 전압(galvanometer 경우, 0도)으로 초기화
	voltage_A = 0;
	voltage_B = 0;

};

uint16_t dac8562_voltageConvert(float voltage)
{
	if (voltage > 12 && voltage <-12){
		return 0;
	}

	float gain = 4.16667; // OPA2277 Circuit : 100 [k-ohm] / 24 [k-ohm] = 4.16667
	float Vref = 2.5; // internal reference voltage : 2.5 [V]

	//Voltage = G * Vref * (2 * Din/65536 -1 )
	uint16_t Din=32768*voltage/gain/Vref+32768;

	return Din;
};

#ifdef _USE_HW_CLI
void cliDAC8562(cli_args_t *args){
	  bool ret = true;


	  if (args->argc == 1){
	    if(args->isStr(0, "get_value") == true){
	            cliPrintf("dac8562, Vout of A channel: %.3f [V], Vout of B channel: %.3f [V]\n", voltage_A, voltage_B);
	    }
	    else{
	    	ret= false;
	    }
	  }
	  else if (args->argc == 3){
	    if(args->isStr(0, "set_value") == true){
	        char channel = *(char *)args->getStr(1);
	        uint16_t voltage = (uint32_t)args->getData(2);
	        if (channel == 0x41){ //"A"
		    	dac8562_writeVoltage_A(voltage);
	        }
	        else if (channel == 0x42){ //"B"
	        	dac8562_writeVoltage_B(voltage);
	        }
	        else if (channel == 0x43){ //"C" A and B
	        	dac8562_writeVoltage_AB(voltage);
	        }
	        else {
	        	ret= false;
	        }
	    }
	    else{
	      ret = false;
	    }
	  }
	  else{
	    ret = false;
	  }

	  if (ret == false)
	  {
	    cliPrintf( "dac8562 get_value \n");
	    cliPrintf( "dac8562 set_value [channel] [voltage] \n channel: A, B, C(AB) \n voltage: -10~10V");
	  }
}
#endif
