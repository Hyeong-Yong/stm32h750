################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/hw/driver/button.c \
../src/hw/driver/cli.c \
../src/hw/driver/fatfs.c \
../src/hw/driver/flash.c \
../src/hw/driver/gpio.c \
../src/hw/driver/lcd.c \
../src/hw/driver/led.c \
../src/hw/driver/ltdc.c \
../src/hw/driver/qspi.c \
../src/hw/driver/resize.c \
../src/hw/driver/sd.c \
../src/hw/driver/sdram.c \
../src/hw/driver/uart.c 

C_DEPS += \
./src/hw/driver/button.d \
./src/hw/driver/cli.d \
./src/hw/driver/fatfs.d \
./src/hw/driver/flash.d \
./src/hw/driver/gpio.d \
./src/hw/driver/lcd.d \
./src/hw/driver/led.d \
./src/hw/driver/ltdc.d \
./src/hw/driver/qspi.d \
./src/hw/driver/resize.d \
./src/hw/driver/sd.d \
./src/hw/driver/sdram.d \
./src/hw/driver/uart.d 

OBJS += \
./src/hw/driver/button.o \
./src/hw/driver/cli.o \
./src/hw/driver/fatfs.o \
./src/hw/driver/flash.o \
./src/hw/driver/gpio.o \
./src/hw/driver/lcd.o \
./src/hw/driver/led.o \
./src/hw/driver/ltdc.o \
./src/hw/driver/qspi.o \
./src/hw/driver/resize.o \
./src/hw/driver/sd.o \
./src/hw/driver/sdram.o \
./src/hw/driver/uart.o 


# Each subdirectory must supply rules for building sources it contributes
src/hw/driver/%.o src/hw/driver/%.su: ../src/hw/driver/%.c src/hw/driver/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DSTM32H750xx -c -I"G:/STM32/stm32h750/ART-Pi/art_pi_fw/src" -I"G:/STM32/stm32h750/ART-Pi/art_pi_fw/src/ap" -I"G:/STM32/stm32h750/ART-Pi/art_pi_fw/src/bsp" -I"G:/STM32/stm32h750/ART-Pi/art_pi_fw/src/common" -I"G:/STM32/stm32h750/ART-Pi/art_pi_fw/src/common/core" -I"G:/STM32/stm32h750/ART-Pi/art_pi_fw/src/common/hw/include" -I"G:/STM32/stm32h750/ART-Pi/art_pi_fw/src/hw" -I"G:/STM32/stm32h750/ART-Pi/art_pi_fw/src/lib" -I"G:/STM32/stm32h750/ART-Pi/art_pi_fw/src/lib/cube_h750/Drivers/CMSIS/Device/ST/STM32H7xx/Include" -I"G:/STM32/stm32h750/ART-Pi/art_pi_fw/src/lib/cube_h750/Drivers/CMSIS/Include" -I"G:/STM32/stm32h750/ART-Pi/art_pi_fw/src/lib/cube_h750/Drivers/STM32H7xx_HAL_Driver/Inc" -I"G:/STM32/stm32h750/ART-Pi/art_pi_fw/src/hw/driver/fatfs" -I"G:/STM32/stm32h750/ART-Pi/art_pi_fw/src/lib/FatFs/src" -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-src-2f-hw-2f-driver

clean-src-2f-hw-2f-driver:
	-$(RM) ./src/hw/driver/button.d ./src/hw/driver/button.o ./src/hw/driver/button.su ./src/hw/driver/cli.d ./src/hw/driver/cli.o ./src/hw/driver/cli.su ./src/hw/driver/fatfs.d ./src/hw/driver/fatfs.o ./src/hw/driver/fatfs.su ./src/hw/driver/flash.d ./src/hw/driver/flash.o ./src/hw/driver/flash.su ./src/hw/driver/gpio.d ./src/hw/driver/gpio.o ./src/hw/driver/gpio.su ./src/hw/driver/lcd.d ./src/hw/driver/lcd.o ./src/hw/driver/lcd.su ./src/hw/driver/led.d ./src/hw/driver/led.o ./src/hw/driver/led.su ./src/hw/driver/ltdc.d ./src/hw/driver/ltdc.o ./src/hw/driver/ltdc.su ./src/hw/driver/qspi.d ./src/hw/driver/qspi.o ./src/hw/driver/qspi.su ./src/hw/driver/resize.d ./src/hw/driver/resize.o ./src/hw/driver/resize.su ./src/hw/driver/sd.d ./src/hw/driver/sd.o ./src/hw/driver/sd.su ./src/hw/driver/sdram.d ./src/hw/driver/sdram.o ./src/hw/driver/sdram.su ./src/hw/driver/uart.d ./src/hw/driver/uart.o ./src/hw/driver/uart.su

.PHONY: clean-src-2f-hw-2f-driver

