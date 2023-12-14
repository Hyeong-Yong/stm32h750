/*
 * dac8562.h
 *
 *  Created on: 2023. 12. 9.
 *      Author: User
 */

#ifndef SRC_HW_DRIVER_SPI_DAC8562_H_
#define SRC_HW_DRIVER_SPI_DAC8562_H_



//WRITE DATA, CMD(DB23-DB16)+DATA(DB15-DB0)
#define CMD_SETA_UPDATEA          0x18    // Write to DAC-A input register and update DAC-A
#define CMD_SETB_UPDATEB          0x19    // Write to DAC-B input register and update DAC-B
#define CMD_UPDATE_ALL_DACS       0x0F    // Update all DACs
//      DATA                      DB16-DB0   16bit DAC data

//SET GAIN, CMD(DB23-DB16)+DATA(DB15-DB0)
#define CMD_GAIN                  0x02    // Gain 입력
#define DATA_GAIN_A1_B1           0x0003  // Gain: DAC-A gain = 1, DAC-B gain = 1 (power-on default
#define DATA_GAIN_A1_B2           0x0001  // Gain: DAC-A gain = 1, DAC-B gain = 2
#define DATA_GAIN_A2_B1           0x0002  // Gain: DAC-A gain = 2, DAC-B gain = 1
#define DATA_GAIN_A2_B2           0x0000  // Gain: DAC-A gain = 2, DAC-B gain = 2 (default with internal VREF)

//POWER ON, CMD(DB23-DB16)+DATA(DB15-DB0)
#define CMD_PWR_UP_A_B            0x20    // CMD: A 및 B 채널 전원 켜기
#define DATA_PWR_UP_A_B           0x0003  // DATA: DAC-A 및 DAC-B 데이터 전원 켜기

//RESET, CMD(DB23-DB16)+DATA(DB15-DB0)
#define CMD_RESET_ALL_REG         0x28  // CMD: 모든 레지스터 재설정, 레지스터 지우기
#define DATA_RESET_ALL_REG        0x0001  // DATA: 모든 레지스터 재설정, 레지스터 지우기

//LDAC, CMD(DB23-DB16)+DATA(DB15-DB0)
#define CMD_LDAC_DIS              0x30  // CMD: LDAC 핀 ​​기능 명령
#define DATA_LDAC_DIS             0x0003  // DATA: Bar_LDAC pin inactive for DAC-B and DAC-A

//Disable internal reference voltage(2.5 V), CMD(DB23-DB16)+DATA(DB15-DB0)
#define CMD_INTERNAL_REF_DIS      0x38  // CMD：Disable internal reference and reset DACs to gain = 1
#define DATA_INTERNAL_REF_DIS     0x0000  // 데이터：Disable internal reference and reset DACs to gain = 1

//Enable internal reference voltage(2.5 V), CMD(DB23-DB16)+DATA(DB15-DB0)
#define CMD_INTERNAL_REF_EN       0x38  // 명령：Enable Internal Reference & reset DACs to gain = 2
#define DATA_INTERNAL_REF_EN      0x0001  // 데이터：Enable Internal Reference & reset DACs to gain = 2




void dac8562_init();
void dac8562_initialize();
void dac8562_writeReg(uint8_t cmd_byte, uint16_t data_byte );
void dac8562_outputValue(uint8_t cmd_byte, uint16_t input);
void dac8562_writeVoltage_AB(float input);
void dac8562_writeValue(uint8_t cmd_byte, uint8_t mid, uint8_t last);
void dac8562_writeVoltage_A(float input);
void dac8562_writeVoltage_B(float input);
uint16_t dac8562_voltageConvert(float voltage);




#endif /* SRC_HW_DRIVER_SPI_DAC8562_H_ */
