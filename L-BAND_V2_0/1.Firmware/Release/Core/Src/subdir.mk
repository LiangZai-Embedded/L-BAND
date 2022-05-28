################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/FastMath.c \
../Core/Src/adc.c \
../Core/Src/bmp280.c \
../Core/Src/bsp_u8g2.c \
../Core/Src/filter.c \
../Core/Src/gpio.c \
../Core/Src/i2c.c \
../Core/Src/imu.c \
../Core/Src/lsm303dlhc.c \
../Core/Src/main.c \
../Core/Src/pedometre.c \
../Core/Src/rtc.c \
../Core/Src/snake.c \
../Core/Src/spi.c \
../Core/Src/stm32g0xx_hal_msp.c \
../Core/Src/stm32g0xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32g0xx.c \
../Core/Src/task.c \
../Core/Src/tim.c \
../Core/Src/ui.c 

OBJS += \
./Core/Src/FastMath.o \
./Core/Src/adc.o \
./Core/Src/bmp280.o \
./Core/Src/bsp_u8g2.o \
./Core/Src/filter.o \
./Core/Src/gpio.o \
./Core/Src/i2c.o \
./Core/Src/imu.o \
./Core/Src/lsm303dlhc.o \
./Core/Src/main.o \
./Core/Src/pedometre.o \
./Core/Src/rtc.o \
./Core/Src/snake.o \
./Core/Src/spi.o \
./Core/Src/stm32g0xx_hal_msp.o \
./Core/Src/stm32g0xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32g0xx.o \
./Core/Src/task.o \
./Core/Src/tim.o \
./Core/Src/ui.o 

C_DEPS += \
./Core/Src/FastMath.d \
./Core/Src/adc.d \
./Core/Src/bmp280.d \
./Core/Src/bsp_u8g2.d \
./Core/Src/filter.d \
./Core/Src/gpio.d \
./Core/Src/i2c.d \
./Core/Src/imu.d \
./Core/Src/lsm303dlhc.d \
./Core/Src/main.d \
./Core/Src/pedometre.d \
./Core/Src/rtc.d \
./Core/Src/snake.d \
./Core/Src/spi.d \
./Core/Src/stm32g0xx_hal_msp.d \
./Core/Src/stm32g0xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32g0xx.d \
./Core/Src/task.d \
./Core/Src/tim.d \
./Core/Src/ui.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -DUSE_HAL_DRIVER -DSTM32G031xx -c -I../Core/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G0xx/Include -I../Drivers/CMSIS/Include -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/FastMath.d ./Core/Src/FastMath.o ./Core/Src/adc.d ./Core/Src/adc.o ./Core/Src/bmp280.d ./Core/Src/bmp280.o ./Core/Src/bsp_u8g2.d ./Core/Src/bsp_u8g2.o ./Core/Src/filter.d ./Core/Src/filter.o ./Core/Src/gpio.d ./Core/Src/gpio.o ./Core/Src/i2c.d ./Core/Src/i2c.o ./Core/Src/imu.d ./Core/Src/imu.o ./Core/Src/lsm303dlhc.d ./Core/Src/lsm303dlhc.o ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/pedometre.d ./Core/Src/pedometre.o ./Core/Src/rtc.d ./Core/Src/rtc.o ./Core/Src/snake.d ./Core/Src/snake.o ./Core/Src/spi.d ./Core/Src/spi.o ./Core/Src/stm32g0xx_hal_msp.d ./Core/Src/stm32g0xx_hal_msp.o ./Core/Src/stm32g0xx_it.d ./Core/Src/stm32g0xx_it.o ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/system_stm32g0xx.d ./Core/Src/system_stm32g0xx.o ./Core/Src/task.d ./Core/Src/task.o ./Core/Src/tim.d ./Core/Src/tim.o ./Core/Src/ui.d ./Core/Src/ui.o

.PHONY: clean-Core-2f-Src

