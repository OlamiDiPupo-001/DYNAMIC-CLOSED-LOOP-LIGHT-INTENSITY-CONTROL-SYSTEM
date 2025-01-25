################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Components/Src/__template.c \
../Components/Src/bh1750.c \
../Components/Src/bh1750_config.c \
../Components/Src/btn.c \
../Components/Src/btn_config.c \
../Components/Src/dio.c \
../Components/Src/disp.c \
../Components/Src/disp_config.c \
../Components/Src/lcd.c \
../Components/Src/lcd_config.c \
../Components/Src/led.c \
../Components/Src/led_config.c \
../Components/Src/menu.c \
../Components/Src/menu_config.c \
../Components/Src/pwm.c 

OBJS += \
./Components/Src/__template.o \
./Components/Src/bh1750.o \
./Components/Src/bh1750_config.o \
./Components/Src/btn.o \
./Components/Src/btn_config.o \
./Components/Src/dio.o \
./Components/Src/disp.o \
./Components/Src/disp_config.o \
./Components/Src/lcd.o \
./Components/Src/lcd_config.o \
./Components/Src/led.o \
./Components/Src/led_config.o \
./Components/Src/menu.o \
./Components/Src/menu_config.o \
./Components/Src/pwm.o 

C_DEPS += \
./Components/Src/__template.d \
./Components/Src/bh1750.d \
./Components/Src/bh1750_config.d \
./Components/Src/btn.d \
./Components/Src/btn_config.d \
./Components/Src/dio.d \
./Components/Src/disp.d \
./Components/Src/disp_config.d \
./Components/Src/lcd.d \
./Components/Src/lcd_config.d \
./Components/Src/led.d \
./Components/Src/led_config.d \
./Components/Src/menu.d \
./Components/Src/menu_config.d \
./Components/Src/pwm.d 


# Each subdirectory must supply rules for building sources it contributes
Components/Src/%.o Components/Src/%.su Components/Src/%.cyclo: ../Components/Src/%.c Components/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F746xx -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/oladi/OneDrive/Desktop/SEMS/MicroProcessor/Final_Project/DYNAMIC-CLOSED-LOOP-LIGHT-INTENSITY-CONTROL-SYSTEM/DCLLI_Control_System/Components/Inc" -I../Middlewares/Third_Party/ARM_CMSIS/CMSIS/Core/Include/ -I../Middlewares/Third_Party/ARM_CMSIS/CMSIS/Core_A/Include/ -I../Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Components-2f-Src

clean-Components-2f-Src:
	-$(RM) ./Components/Src/__template.cyclo ./Components/Src/__template.d ./Components/Src/__template.o ./Components/Src/__template.su ./Components/Src/bh1750.cyclo ./Components/Src/bh1750.d ./Components/Src/bh1750.o ./Components/Src/bh1750.su ./Components/Src/bh1750_config.cyclo ./Components/Src/bh1750_config.d ./Components/Src/bh1750_config.o ./Components/Src/bh1750_config.su ./Components/Src/btn.cyclo ./Components/Src/btn.d ./Components/Src/btn.o ./Components/Src/btn.su ./Components/Src/btn_config.cyclo ./Components/Src/btn_config.d ./Components/Src/btn_config.o ./Components/Src/btn_config.su ./Components/Src/dio.cyclo ./Components/Src/dio.d ./Components/Src/dio.o ./Components/Src/dio.su ./Components/Src/disp.cyclo ./Components/Src/disp.d ./Components/Src/disp.o ./Components/Src/disp.su ./Components/Src/disp_config.cyclo ./Components/Src/disp_config.d ./Components/Src/disp_config.o ./Components/Src/disp_config.su ./Components/Src/lcd.cyclo ./Components/Src/lcd.d ./Components/Src/lcd.o ./Components/Src/lcd.su ./Components/Src/lcd_config.cyclo ./Components/Src/lcd_config.d ./Components/Src/lcd_config.o ./Components/Src/lcd_config.su ./Components/Src/led.cyclo ./Components/Src/led.d ./Components/Src/led.o ./Components/Src/led.su ./Components/Src/led_config.cyclo ./Components/Src/led_config.d ./Components/Src/led_config.o ./Components/Src/led_config.su ./Components/Src/menu.cyclo ./Components/Src/menu.d ./Components/Src/menu.o ./Components/Src/menu.su ./Components/Src/menu_config.cyclo ./Components/Src/menu_config.d ./Components/Src/menu_config.o ./Components/Src/menu_config.su ./Components/Src/pwm.cyclo ./Components/Src/pwm.d ./Components/Src/pwm.o ./Components/Src/pwm.su

.PHONY: clean-Components-2f-Src

