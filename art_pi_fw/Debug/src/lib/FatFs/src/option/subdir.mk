################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/lib/FatFs/src/option/syscall.c \
../src/lib/FatFs/src/option/unicode.c 

C_DEPS += \
./src/lib/FatFs/src/option/syscall.d \
./src/lib/FatFs/src/option/unicode.d 

OBJS += \
./src/lib/FatFs/src/option/syscall.o \
./src/lib/FatFs/src/option/unicode.o 


# Each subdirectory must supply rules for building sources it contributes
src/lib/FatFs/src/option/%.o src/lib/FatFs/src/option/%.su: ../src/lib/FatFs/src/option/%.c src/lib/FatFs/src/option/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DSTM32H750xx -c -I"G:/STM32/stm32h750/ART-Pi/art_pi_fw/src" -I"G:/STM32/stm32h750/ART-Pi/art_pi_fw/src/ap" -I"G:/STM32/stm32h750/ART-Pi/art_pi_fw/src/bsp" -I"G:/STM32/stm32h750/ART-Pi/art_pi_fw/src/common" -I"G:/STM32/stm32h750/ART-Pi/art_pi_fw/src/common/core" -I"G:/STM32/stm32h750/ART-Pi/art_pi_fw/src/common/hw/include" -I"G:/STM32/stm32h750/ART-Pi/art_pi_fw/src/hw" -I"G:/STM32/stm32h750/ART-Pi/art_pi_fw/src/lib" -I"G:/STM32/stm32h750/ART-Pi/art_pi_fw/src/lib/cube_h750/Drivers/CMSIS/Device/ST/STM32H7xx/Include" -I"G:/STM32/stm32h750/ART-Pi/art_pi_fw/src/lib/cube_h750/Drivers/CMSIS/Include" -I"G:/STM32/stm32h750/ART-Pi/art_pi_fw/src/lib/cube_h750/Drivers/STM32H7xx_HAL_Driver/Inc" -I"G:/STM32/stm32h750/ART-Pi/art_pi_fw/src/hw/driver/fatfs" -I"G:/STM32/stm32h750/ART-Pi/art_pi_fw/src/lib/FatFs/src" -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-src-2f-lib-2f-FatFs-2f-src-2f-option

clean-src-2f-lib-2f-FatFs-2f-src-2f-option:
	-$(RM) ./src/lib/FatFs/src/option/syscall.d ./src/lib/FatFs/src/option/syscall.o ./src/lib/FatFs/src/option/syscall.su ./src/lib/FatFs/src/option/unicode.d ./src/lib/FatFs/src/option/unicode.o ./src/lib/FatFs/src/option/unicode.su

.PHONY: clean-src-2f-lib-2f-FatFs-2f-src-2f-option

