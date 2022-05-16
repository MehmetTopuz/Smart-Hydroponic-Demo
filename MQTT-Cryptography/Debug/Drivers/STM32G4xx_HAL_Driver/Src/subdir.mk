################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal.c \
../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_cortex.c \
../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_dma.c \
../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_dma_ex.c \
../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_exti.c \
../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_flash.c \
../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_flash_ex.c \
../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_flash_ramfunc.c \
../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_gpio.c \
../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_pwr.c \
../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_pwr_ex.c \
../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_rcc.c \
../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_rcc_ex.c \
../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_tim.c \
../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_tim_ex.c \
../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_uart.c \
../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_uart_ex.c 

C_DEPS += \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal.d \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_cortex.d \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_dma.d \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_dma_ex.d \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_exti.d \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_flash.d \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_flash_ex.d \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_flash_ramfunc.d \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_gpio.d \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_pwr.d \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_pwr_ex.d \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_rcc.d \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_rcc_ex.d \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_tim.d \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_tim_ex.d \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_uart.d \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_uart_ex.d 

OBJS += \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal.o \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_cortex.o \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_dma.o \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_dma_ex.o \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_exti.o \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_flash.o \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_flash_ex.o \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_flash_ramfunc.o \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_gpio.o \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_pwr.o \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_pwr_ex.o \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_rcc.o \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_rcc_ex.o \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_tim.o \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_tim_ex.o \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_uart.o \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_uart_ex.o 


# Each subdirectory must supply rules for building sources it contributes
Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal.o: ../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal.c Drivers/STM32G4xx_HAL_Driver/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G474xx -c -I../Core/Inc -I"C:/Users/Topuz/STM32CubeIDE/workspace_1.6.0/MQTT/MQTT-Cryptography/ExampleLib" -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Topuz/STM32CubeIDE/workspace_1.6.0/MQTT/MQTT-Cryptography/ESP8266_Driver" -I"C:/Users/Topuz/STM32CubeIDE/workspace_1.6.0/MQTT/MQTT-Cryptography/MQTT_Library" -I"C:/Users/Topuz/STM32CubeIDE/workspace_1.6.0/MQTT/MQTT-Cryptography/RingBuffer" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_cortex.o: ../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_cortex.c Drivers/STM32G4xx_HAL_Driver/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G474xx -c -I../Core/Inc -I"C:/Users/Topuz/STM32CubeIDE/workspace_1.6.0/MQTT/MQTT-Cryptography/ExampleLib" -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Topuz/STM32CubeIDE/workspace_1.6.0/MQTT/MQTT-Cryptography/ESP8266_Driver" -I"C:/Users/Topuz/STM32CubeIDE/workspace_1.6.0/MQTT/MQTT-Cryptography/MQTT_Library" -I"C:/Users/Topuz/STM32CubeIDE/workspace_1.6.0/MQTT/MQTT-Cryptography/RingBuffer" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_cortex.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_dma.o: ../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_dma.c Drivers/STM32G4xx_HAL_Driver/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G474xx -c -I../Core/Inc -I"C:/Users/Topuz/STM32CubeIDE/workspace_1.6.0/MQTT/MQTT-Cryptography/ExampleLib" -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Topuz/STM32CubeIDE/workspace_1.6.0/MQTT/MQTT-Cryptography/ESP8266_Driver" -I"C:/Users/Topuz/STM32CubeIDE/workspace_1.6.0/MQTT/MQTT-Cryptography/MQTT_Library" -I"C:/Users/Topuz/STM32CubeIDE/workspace_1.6.0/MQTT/MQTT-Cryptography/RingBuffer" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_dma.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_dma_ex.o: ../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_dma_ex.c Drivers/STM32G4xx_HAL_Driver/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G474xx -c -I../Core/Inc -I"C:/Users/Topuz/STM32CubeIDE/workspace_1.6.0/MQTT/MQTT-Cryptography/ExampleLib" -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Topuz/STM32CubeIDE/workspace_1.6.0/MQTT/MQTT-Cryptography/ESP8266_Driver" -I"C:/Users/Topuz/STM32CubeIDE/workspace_1.6.0/MQTT/MQTT-Cryptography/MQTT_Library" -I"C:/Users/Topuz/STM32CubeIDE/workspace_1.6.0/MQTT/MQTT-Cryptography/RingBuffer" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_dma_ex.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_exti.o: ../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_exti.c Drivers/STM32G4xx_HAL_Driver/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G474xx -c -I../Core/Inc -I"C:/Users/Topuz/STM32CubeIDE/workspace_1.6.0/MQTT/MQTT-Cryptography/ExampleLib" -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Topuz/STM32CubeIDE/workspace_1.6.0/MQTT/MQTT-Cryptography/ESP8266_Driver" -I"C:/Users/Topuz/STM32CubeIDE/workspace_1.6.0/MQTT/MQTT-Cryptography/MQTT_Library" -I"C:/Users/Topuz/STM32CubeIDE/workspace_1.6.0/MQTT/MQTT-Cryptography/RingBuffer" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_exti.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_flash.o: ../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_flash.c Drivers/STM32G4xx_HAL_Driver/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G474xx -c -I../Core/Inc -I"C:/Users/Topuz/STM32CubeIDE/workspace_1.6.0/MQTT/MQTT-Cryptography/ExampleLib" -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Topuz/STM32CubeIDE/workspace_1.6.0/MQTT/MQTT-Cryptography/ESP8266_Driver" -I"C:/Users/Topuz/STM32CubeIDE/workspace_1.6.0/MQTT/MQTT-Cryptography/MQTT_Library" -I"C:/Users/Topuz/STM32CubeIDE/workspace_1.6.0/MQTT/MQTT-Cryptography/RingBuffer" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_flash.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_flash_ex.o: ../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_flash_ex.c Drivers/STM32G4xx_HAL_Driver/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G474xx -c -I../Core/Inc -I"C:/Users/Topuz/STM32CubeIDE/workspace_1.6.0/MQTT/MQTT-Cryptography/ExampleLib" -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Topuz/STM32CubeIDE/workspace_1.6.0/MQTT/MQTT-Cryptography/ESP8266_Driver" -I"C:/Users/Topuz/STM32CubeIDE/workspace_1.6.0/MQTT/MQTT-Cryptography/MQTT_Library" -I"C:/Users/Topuz/STM32CubeIDE/workspace_1.6.0/MQTT/MQTT-Cryptography/RingBuffer" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_flash_ex.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_flash_ramfunc.o: ../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_flash_ramfunc.c Drivers/STM32G4xx_HAL_Driver/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G474xx -c -I../Core/Inc -I"C:/Users/Topuz/STM32CubeIDE/workspace_1.6.0/MQTT/MQTT-Cryptography/ExampleLib" -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Topuz/STM32CubeIDE/workspace_1.6.0/MQTT/MQTT-Cryptography/ESP8266_Driver" -I"C:/Users/Topuz/STM32CubeIDE/workspace_1.6.0/MQTT/MQTT-Cryptography/MQTT_Library" -I"C:/Users/Topuz/STM32CubeIDE/workspace_1.6.0/MQTT/MQTT-Cryptography/RingBuffer" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_flash_ramfunc.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_gpio.o: ../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_gpio.c Drivers/STM32G4xx_HAL_Driver/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G474xx -c -I../Core/Inc -I"C:/Users/Topuz/STM32CubeIDE/workspace_1.6.0/MQTT/MQTT-Cryptography/ExampleLib" -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Topuz/STM32CubeIDE/workspace_1.6.0/MQTT/MQTT-Cryptography/ESP8266_Driver" -I"C:/Users/Topuz/STM32CubeIDE/workspace_1.6.0/MQTT/MQTT-Cryptography/MQTT_Library" -I"C:/Users/Topuz/STM32CubeIDE/workspace_1.6.0/MQTT/MQTT-Cryptography/RingBuffer" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_gpio.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_pwr.o: ../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_pwr.c Drivers/STM32G4xx_HAL_Driver/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G474xx -c -I../Core/Inc -I"C:/Users/Topuz/STM32CubeIDE/workspace_1.6.0/MQTT/MQTT-Cryptography/ExampleLib" -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Topuz/STM32CubeIDE/workspace_1.6.0/MQTT/MQTT-Cryptography/ESP8266_Driver" -I"C:/Users/Topuz/STM32CubeIDE/workspace_1.6.0/MQTT/MQTT-Cryptography/MQTT_Library" -I"C:/Users/Topuz/STM32CubeIDE/workspace_1.6.0/MQTT/MQTT-Cryptography/RingBuffer" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_pwr.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_pwr_ex.o: ../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_pwr_ex.c Drivers/STM32G4xx_HAL_Driver/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G474xx -c -I../Core/Inc -I"C:/Users/Topuz/STM32CubeIDE/workspace_1.6.0/MQTT/MQTT-Cryptography/ExampleLib" -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Topuz/STM32CubeIDE/workspace_1.6.0/MQTT/MQTT-Cryptography/ESP8266_Driver" -I"C:/Users/Topuz/STM32CubeIDE/workspace_1.6.0/MQTT/MQTT-Cryptography/MQTT_Library" -I"C:/Users/Topuz/STM32CubeIDE/workspace_1.6.0/MQTT/MQTT-Cryptography/RingBuffer" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_pwr_ex.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_rcc.o: ../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_rcc.c Drivers/STM32G4xx_HAL_Driver/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G474xx -c -I../Core/Inc -I"C:/Users/Topuz/STM32CubeIDE/workspace_1.6.0/MQTT/MQTT-Cryptography/ExampleLib" -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Topuz/STM32CubeIDE/workspace_1.6.0/MQTT/MQTT-Cryptography/ESP8266_Driver" -I"C:/Users/Topuz/STM32CubeIDE/workspace_1.6.0/MQTT/MQTT-Cryptography/MQTT_Library" -I"C:/Users/Topuz/STM32CubeIDE/workspace_1.6.0/MQTT/MQTT-Cryptography/RingBuffer" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_rcc.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_rcc_ex.o: ../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_rcc_ex.c Drivers/STM32G4xx_HAL_Driver/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G474xx -c -I../Core/Inc -I"C:/Users/Topuz/STM32CubeIDE/workspace_1.6.0/MQTT/MQTT-Cryptography/ExampleLib" -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Topuz/STM32CubeIDE/workspace_1.6.0/MQTT/MQTT-Cryptography/ESP8266_Driver" -I"C:/Users/Topuz/STM32CubeIDE/workspace_1.6.0/MQTT/MQTT-Cryptography/MQTT_Library" -I"C:/Users/Topuz/STM32CubeIDE/workspace_1.6.0/MQTT/MQTT-Cryptography/RingBuffer" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_rcc_ex.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_tim.o: ../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_tim.c Drivers/STM32G4xx_HAL_Driver/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G474xx -c -I../Core/Inc -I"C:/Users/Topuz/STM32CubeIDE/workspace_1.6.0/MQTT/MQTT-Cryptography/ExampleLib" -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Topuz/STM32CubeIDE/workspace_1.6.0/MQTT/MQTT-Cryptography/ESP8266_Driver" -I"C:/Users/Topuz/STM32CubeIDE/workspace_1.6.0/MQTT/MQTT-Cryptography/MQTT_Library" -I"C:/Users/Topuz/STM32CubeIDE/workspace_1.6.0/MQTT/MQTT-Cryptography/RingBuffer" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_tim.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_tim_ex.o: ../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_tim_ex.c Drivers/STM32G4xx_HAL_Driver/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G474xx -c -I../Core/Inc -I"C:/Users/Topuz/STM32CubeIDE/workspace_1.6.0/MQTT/MQTT-Cryptography/ExampleLib" -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Topuz/STM32CubeIDE/workspace_1.6.0/MQTT/MQTT-Cryptography/ESP8266_Driver" -I"C:/Users/Topuz/STM32CubeIDE/workspace_1.6.0/MQTT/MQTT-Cryptography/MQTT_Library" -I"C:/Users/Topuz/STM32CubeIDE/workspace_1.6.0/MQTT/MQTT-Cryptography/RingBuffer" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_tim_ex.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_uart.o: ../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_uart.c Drivers/STM32G4xx_HAL_Driver/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G474xx -c -I../Core/Inc -I"C:/Users/Topuz/STM32CubeIDE/workspace_1.6.0/MQTT/MQTT-Cryptography/ExampleLib" -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Topuz/STM32CubeIDE/workspace_1.6.0/MQTT/MQTT-Cryptography/ESP8266_Driver" -I"C:/Users/Topuz/STM32CubeIDE/workspace_1.6.0/MQTT/MQTT-Cryptography/MQTT_Library" -I"C:/Users/Topuz/STM32CubeIDE/workspace_1.6.0/MQTT/MQTT-Cryptography/RingBuffer" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_uart.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_uart_ex.o: ../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_uart_ex.c Drivers/STM32G4xx_HAL_Driver/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G474xx -c -I../Core/Inc -I"C:/Users/Topuz/STM32CubeIDE/workspace_1.6.0/MQTT/MQTT-Cryptography/ExampleLib" -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Topuz/STM32CubeIDE/workspace_1.6.0/MQTT/MQTT-Cryptography/ESP8266_Driver" -I"C:/Users/Topuz/STM32CubeIDE/workspace_1.6.0/MQTT/MQTT-Cryptography/MQTT_Library" -I"C:/Users/Topuz/STM32CubeIDE/workspace_1.6.0/MQTT/MQTT-Cryptography/RingBuffer" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_uart_ex.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

