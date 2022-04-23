################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../UnitTests/esp8266_test.cpp \
../UnitTests/main.cpp \
../UnitTests/mqtt_test.cpp 

OBJS += \
./UnitTests/esp8266_test.o \
./UnitTests/main.o \
./UnitTests/mqtt_test.o 

CPP_DEPS += \
./UnitTests/esp8266_test.d \
./UnitTests/main.d \
./UnitTests/mqtt_test.d 


# Each subdirectory must supply rules for building sources it contributes
UnitTests/esp8266_test.o: ../UnitTests/esp8266_test.cpp UnitTests/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DDEBUG -DCPPUTEST_STD_CPP_LIB_DISABLED -DCPPUTEST_STD_C_LIB_DISABLED -DUSE_HAL_DRIVER -DSTM32G474xx -c -I../Core/Inc -I"C:/Users/Topuz/STM32CubeIDE/workspace_1.6.0/MQTT/MQTT-Cryptography/UnitTests/include" -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Topuz/STM32CubeIDE/workspace_1.6.0/MQTT/MQTT-Cryptography/RingBuffer" -I"C:/Users/Topuz/STM32CubeIDE/workspace_1.6.0/MQTT/MQTT-Cryptography/MQTT_Library" -I"C:/Users/Topuz/STM32CubeIDE/workspace_1.6.0/MQTT/MQTT-Cryptography/ESP8266_Driver" -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"UnitTests/esp8266_test.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
UnitTests/main.o: ../UnitTests/main.cpp UnitTests/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DDEBUG -DCPPUTEST_STD_CPP_LIB_DISABLED -DCPPUTEST_STD_C_LIB_DISABLED -DUSE_HAL_DRIVER -DSTM32G474xx -c -I../Core/Inc -I"C:/Users/Topuz/STM32CubeIDE/workspace_1.6.0/MQTT/MQTT-Cryptography/UnitTests/include" -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Topuz/STM32CubeIDE/workspace_1.6.0/MQTT/MQTT-Cryptography/RingBuffer" -I"C:/Users/Topuz/STM32CubeIDE/workspace_1.6.0/MQTT/MQTT-Cryptography/MQTT_Library" -I"C:/Users/Topuz/STM32CubeIDE/workspace_1.6.0/MQTT/MQTT-Cryptography/ESP8266_Driver" -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"UnitTests/main.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
UnitTests/mqtt_test.o: ../UnitTests/mqtt_test.cpp UnitTests/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DDEBUG -DCPPUTEST_STD_CPP_LIB_DISABLED -DCPPUTEST_STD_C_LIB_DISABLED -DUSE_HAL_DRIVER -DSTM32G474xx -c -I../Core/Inc -I"C:/Users/Topuz/STM32CubeIDE/workspace_1.6.0/MQTT/MQTT-Cryptography/UnitTests/include" -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Topuz/STM32CubeIDE/workspace_1.6.0/MQTT/MQTT-Cryptography/RingBuffer" -I"C:/Users/Topuz/STM32CubeIDE/workspace_1.6.0/MQTT/MQTT-Cryptography/MQTT_Library" -I"C:/Users/Topuz/STM32CubeIDE/workspace_1.6.0/MQTT/MQTT-Cryptography/ESP8266_Driver" -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"UnitTests/mqtt_test.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

