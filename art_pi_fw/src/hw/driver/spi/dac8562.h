/*
 * dac8562.h
 *
 *  Created on: 2023. 12. 9.
 *      Author: User
 */

#ifndef SRC_HW_DRIVER_SPI_DAC8562_H_
#define SRC_HW_DRIVER_SPI_DAC8562_H_



#define CMD_SETA_UPDATEA          0x18  // A 채널 명령 + 16비트 A 채널 데이터
#define CMD_SETB_UPDATEB          0x19  // B 채널 명령 + 16비트 B 채널 데이터
#define CMD_UPDATE_ALL_DACS       0x0F  // 양방향 레지스터 업데이트 명령, 마지막 16비트에는 시계만 필요

#define CMD_GAIN                  0x02  // 내부 Gain 명령
#define DATA_GAIN_B2_A2           0x0000  // 채널 B에 대해 2x, 채널 A에 대해 1x
#define DATA_GAIN_B2_A1           0x0001  // 채널 B에 대해 1x, 채널 A에 대해 2x
#define DATA_GAIN_B1_A2           0x0002  // 채널 B는 2x, 채널 A는 2x
#define DATA_GAIN_B1_A1           0x0003  // 채널 B의 경우 1x, 채널 A의 경우 1x

#define CMD_PWR_UP_A_B            0x20  // 명령: A 및 B 채널 전원 켜기
#define DATA_PWR_UP_A_B           0x0003  // 데이터: DAC-A 및 DAC-B 데이터 전원 켜기

#define CMD_RESET_ALL_REG         0x28  // 명령: 모든 레지스터 재설정, 레지스터 지우기
#define DATA_RESET_ALL_REG        0x0001  // 데이터: 모든 레지스터 재설정, 레지스터 지우기

#define CMD_LDAC_DIS              0x30  // LDAC 핀 ​​기능 명령
#define DATA_LDAC_DIS             0x0003  // LDAC 핀이 작동하지 않습니다

#define CMD_INTERNAL_REF_DIS      0x38  // 명령：Disable internal reference and reset DACs to gain = 1
#define DATA_INTERNAL_REF_DIS     0x0000  // 데이터：Disable internal reference and reset DACs to gain = 1
#define CMD_INTERNAL_REF_EN       0x38  // 명령：Enable Internal Reference & reset DACs to gain = 2
#define DATA_INTERNAL_REF_EN      0x0001  // 데이터：Enable Internal Reference & reset DACs to gain = 2

void dac8562_init();
void dac8562_initialize();
void dac8562_writeReg(uint8_t cmd_byte, uint16_t data_byte );
void dac8562_outputValue(uint8_t cmd_byte, uint16_t input);
void dac8562_writeVoltage(float input);
void dac8562_writeValue(uint8_t cmd_byte, uint8_t mid, uint8_t last);
void writeA(float input);
void writeB(float input);
uint16_t dac8562_voltageConvert(float voltage);




#endif /* SRC_HW_DRIVER_SPI_DAC8562_H_ */
