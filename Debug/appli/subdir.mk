################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../appli/affichageTFT.c \
../appli/main.c \
../appli/mpc9701.c 

OBJS += \
./appli/affichageTFT.o \
./appli/main.o \
./appli/mpc9701.o 

C_DEPS += \
./appli/affichageTFT.d \
./appli/main.d \
./appli/mpc9701.d 


# Each subdirectory must supply rules for building sources it contributes
appli/%.o appli/%.su appli/%.cyclo: ../appli/%.c appli/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32F1 -DNUCLEO_F103RB -DSTM32F103RBTx -DSTM32 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../lib/bsp -I../lib/bsp/MPU6050 -I../lib/bsp/iks01a1 -I../lib/bsp/iks01a1/Common -I../lib/bsp/iks01a1/hts221 -I../lib/bsp/iks01a1/lis3mdl -I../lib/bsp/iks01a1/lps22hb -I../lib/bsp/iks01a1/lps25hb -I../lib/bsp/iks01a1/lsm6ds0 -I../lib/bsp/iks01a1/lsm6ds3 -I../lib/bsp/tft_ili9341 -I../lib/bsp/tft_pcd8544 -I../lib/bsp/MCP23S17 -I../lib/hal/inc -I../lib/bsp/Common -I../lib/bsp/lcd2x16 -I../lib/bsp/MLX90614 -I../lib/bsp/MatrixKeyboard -I../lib/bsp/MatrixLed -I../lib/CMSIS/core -I../lib/CMSIS/device -I../lib/middleware/FatFs -I../lib/middleware/FatFs/src -I../lib/middleware/FatFs/src/drivers -I../appli -Og -ffunction-sections -fdata-sections -Wall -Wextra -Wconversion -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@"  -mfloat-abi=soft -mthumb -o "$@"

clean: clean-appli

clean-appli:
	-$(RM) ./appli/affichageTFT.cyclo ./appli/affichageTFT.d ./appli/affichageTFT.o ./appli/affichageTFT.su ./appli/main.cyclo ./appli/main.d ./appli/main.o ./appli/main.su ./appli/mpc9701.cyclo ./appli/mpc9701.d ./appli/mpc9701.o ./appli/mpc9701.su

.PHONY: clean-appli

