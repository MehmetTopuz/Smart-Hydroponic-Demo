################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../MQTT_Library/mqtt.c 

C_DEPS += \
./MQTT_Library/mqtt.d 

OBJS += \
./MQTT_Library/mqtt.o 


# Each subdirectory must supply rules for building sources it contributes
MQTT_Library/mqtt.o: ../MQTT_Library/mqtt.c MQTT_Library/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G474xx -c -I../Core/Inc -I"C:/Users/Topuz/STM32CubeIDE/workspace_1.6.0/MQTT/MQTT-Cryptography/ESP8266_Driver" -I"C:/Users/Topuz/STM32CubeIDE/workspace_1.6.0/MQTT/MQTT-Cryptography/MQTT_Library" -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Topuz/STM32CubeIDE/workspace_1.6.0/MQTT/MQTT-Cryptography/RingBuffer" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"MQTT_Library/mqtt.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

