################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/bsp/bsp.c \
../src/bsp/stm32h7xx_hal_msp.c \
../src/bsp/stm32h7xx_it.c \
../src/bsp/syscalls.c \
../src/bsp/sysmem.c \
../src/bsp/system_stm32h7xx.c 

C_DEPS += \
./src/bsp/bsp.d \
./src/bsp/stm32h7xx_hal_msp.d \
./src/bsp/stm32h7xx_it.d \
./src/bsp/syscalls.d \
./src/bsp/sysmem.d \
./src/bsp/system_stm32h7xx.d 

OBJS += \
./src/bsp/bsp.o \
./src/bsp/stm32h7xx_hal_msp.o \
./src/bsp/stm32h7xx_it.o \
./src/bsp/syscalls.o \
./src/bsp/sysmem.o \
./src/bsp/system_stm32h7xx.o 


# Each subdirectory must supply rules for building sources it contributes
src/bsp/%.o src/bsp/%.su: ../src/bsp/%.c src/bsp/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DSTM32H750xx -c -I"G:/STM32/stm32h750/ART-Pi/art_pi_fw/src" -I"G:/STM32/stm32h750/ART-Pi/art_pi_fw/src/ap" -I"G:/STM32/stm32h750/ART-Pi/art_pi_fw/src/bsp" -I"G:/STM32/stm32h750/ART-Pi/art_pi_fw/src/common" -I"G:/STM32/stm32h750/ART-Pi/art_pi_fw/src/common/core" -I"G:/STM32/stm32h750/ART-Pi/art_pi_fw/src/common/hw/include" -I"G:/STM32/stm32h750/ART-Pi/art_pi_fw/src/hw" -I"G:/STM32/stm32h750/ART-Pi/art_pi_fw/src/lib" -I"G:/STM32/stm32h750/ART-Pi/art_pi_fw/src/lib/cube_h750/Drivers/CMSIS/Device/ST/STM32H7xx/Include" -I"G:/STM32/stm32h750/ART-Pi/art_pi_fw/src/lib/cube_h750/Drivers/CMSIS/Include" -I"G:/STM32/stm32h750/ART-Pi/art_pi_fw/src/lib/cube_h750/Drivers/STM32H7xx_HAL_Driver/Inc" -I"G:/STM32/stm32h750/ART-Pi/art_pi_fw/src/hw/driver/fatfs" -I"G:/STM32/stm32h750/ART-Pi/art_pi_fw/src/lib/FatFs/src" -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-src-2f-bsp

clean-src-2f-bsp:
	-$(RM) ./src/bsp/bsp.d ./src/bsp/bsp.o ./src/bsp/bsp.su ./src/bsp/stm32h7xx_hal_msp.d ./src/bsp/stm32h7xx_hal_msp.o ./src/bsp/stm32h7xx_hal_msp.su ./src/bsp/stm32h7xx_it.d ./src/bsp/stm32h7xx_it.o ./src/bsp/stm32h7xx_it.su ./src/bsp/syscalls.d ./src/bsp/syscalls.o ./src/bsp/syscalls.su ./src/bsp/sysmem.d ./src/bsp/sysmem.o ./src/bsp/sysmem.su ./src/bsp/system_stm32h7xx.d ./src/bsp/system_stm32h7xx.o ./src/bsp/system_stm32h7xx.su

.PHONY: clean-src-2f-bsp

