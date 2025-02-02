################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lib/bsp/MPU6050/stm32f1_mpu6050.c 

OBJS += \
./lib/bsp/MPU6050/stm32f1_mpu6050.o 

C_DEPS += \
./lib/bsp/MPU6050/stm32f1_mpu6050.d 


# Each subdirectory must supply rules for building sources it contributes
lib/bsp/MPU6050/%.o lib/bsp/MPU6050/%.su lib/bsp/MPU6050/%.cyclo: ../lib/bsp/MPU6050/%.c lib/bsp/MPU6050/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32F1 -DNUCLEO_F103RB -DSTM32F103RBTx -DSTM32 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../lib/bsp -I../lib/bsp/MPU6050 -I../lib/bsp/iks01a1 -I../lib/bsp/iks01a1/Common -I../lib/bsp/iks01a1/hts221 -I../lib/bsp/iks01a1/lis3mdl -I../lib/bsp/iks01a1/lps22hb -I../lib/bsp/iks01a1/lps25hb -I../lib/bsp/iks01a1/lsm6ds0 -I../lib/bsp/iks01a1/lsm6ds3 -I../lib/bsp/tft_ili9341 -I../lib/bsp/tft_pcd8544 -I../lib/bsp/MCP23S17 -I../lib/hal/inc -I../lib/bsp/Common -I../lib/bsp/lcd2x16 -I../lib/bsp/MLX90614 -I../lib/bsp/MatrixKeyboard -I../lib/bsp/MatrixLed -I../lib/CMSIS/core -I../lib/CMSIS/device -I../lib/middleware/FatFs -I../lib/middleware/FatFs/src -I../lib/middleware/FatFs/src/drivers -I../appli -Og -ffunction-sections -fdata-sections -Wall -Wextra -Wconversion -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@"  -mfloat-abi=soft -mthumb -o "$@"

clean: clean-lib-2f-bsp-2f-MPU6050

clean-lib-2f-bsp-2f-MPU6050:
	-$(RM) ./lib/bsp/MPU6050/stm32f1_mpu6050.cyclo ./lib/bsp/MPU6050/stm32f1_mpu6050.d ./lib/bsp/MPU6050/stm32f1_mpu6050.o ./lib/bsp/MPU6050/stm32f1_mpu6050.su

.PHONY: clean-lib-2f-bsp-2f-MPU6050

