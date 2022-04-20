################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/adc.c \
../Core/Src/analog.c \
../Core/Src/cJSON.c \
../Core/Src/cmd.c \
../Core/Src/com.c \
../Core/Src/delay.c \
../Core/Src/digital.c \
../Core/Src/dma.c \
../Core/Src/ds18b20.c \
../Core/Src/gpio.c \
../Core/Src/gsm.c \
../Core/Src/input_JSON.c \
../Core/Src/input_data.c \
../Core/Src/logic_func.c \
../Core/Src/main.c \
../Core/Src/net.c \
../Core/Src/rs485.c \
../Core/Src/rtc.c \
../Core/Src/spi.c \
../Core/Src/spi_interface.c \
../Core/Src/stm32f4xx_hal_msp.c \
../Core/Src/stm32f4xx_hal_timebase_tim.c \
../Core/Src/stm32f4xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32f4xx.c \
../Core/Src/tim.c \
../Core/Src/usart.c \
../Core/Src/usart_ring.c 

OBJS += \
./Core/Src/adc.o \
./Core/Src/analog.o \
./Core/Src/cJSON.o \
./Core/Src/cmd.o \
./Core/Src/com.o \
./Core/Src/delay.o \
./Core/Src/digital.o \
./Core/Src/dma.o \
./Core/Src/ds18b20.o \
./Core/Src/gpio.o \
./Core/Src/gsm.o \
./Core/Src/input_JSON.o \
./Core/Src/input_data.o \
./Core/Src/logic_func.o \
./Core/Src/main.o \
./Core/Src/net.o \
./Core/Src/rs485.o \
./Core/Src/rtc.o \
./Core/Src/spi.o \
./Core/Src/spi_interface.o \
./Core/Src/stm32f4xx_hal_msp.o \
./Core/Src/stm32f4xx_hal_timebase_tim.o \
./Core/Src/stm32f4xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32f4xx.o \
./Core/Src/tim.o \
./Core/Src/usart.o \
./Core/Src/usart_ring.o 

C_DEPS += \
./Core/Src/adc.d \
./Core/Src/analog.d \
./Core/Src/cJSON.d \
./Core/Src/cmd.d \
./Core/Src/com.d \
./Core/Src/delay.d \
./Core/Src/digital.d \
./Core/Src/dma.d \
./Core/Src/ds18b20.d \
./Core/Src/gpio.d \
./Core/Src/gsm.d \
./Core/Src/input_JSON.d \
./Core/Src/input_data.d \
./Core/Src/logic_func.d \
./Core/Src/main.d \
./Core/Src/net.d \
./Core/Src/rs485.d \
./Core/Src/rtc.d \
./Core/Src/spi.d \
./Core/Src/spi_interface.d \
./Core/Src/stm32f4xx_hal_msp.d \
./Core/Src/stm32f4xx_hal_timebase_tim.d \
./Core/Src/stm32f4xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32f4xx.d \
./Core/Src/tim.d \
./Core/Src/usart.d \
./Core/Src/usart_ring.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DDEBUG -DSTM32F407xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/LwIP/src/include/netif/ppp -I../Middlewares/Third_Party/LwIP/src/include/lwip/priv -I../Middlewares/Third_Party/LwIP/src/include/compat/stdc -I../Middlewares/Third_Party/LwIP/src/include/lwip/prot -I../Middlewares/Third_Party/LwIP/src/include/lwip/apps -I../Middlewares/Third_Party/LwIP/src/include/compat/posix/sys -I../Middlewares/Third_Party/LwIP/system/arch -I../Middlewares/Third_Party/LwIP/src/include -I../LWIP/App -I../Middlewares/Third_Party/LwIP/src/include/compat/posix -I../LWIP/Target -I../Middlewares/Third_Party/LwIP/src/include/lwip -I../Middlewares/Third_Party/LwIP/src/include/compat/posix/net -I../Middlewares/Third_Party/LwIP/src/include/compat/posix/arpa -I../Middlewares/Third_Party/LwIP/system -I../Middlewares/Third_Party/LwIP/src/include/netif -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/adc.d ./Core/Src/adc.o ./Core/Src/analog.d ./Core/Src/analog.o ./Core/Src/cJSON.d ./Core/Src/cJSON.o ./Core/Src/cmd.d ./Core/Src/cmd.o ./Core/Src/com.d ./Core/Src/com.o ./Core/Src/delay.d ./Core/Src/delay.o ./Core/Src/digital.d ./Core/Src/digital.o ./Core/Src/dma.d ./Core/Src/dma.o ./Core/Src/ds18b20.d ./Core/Src/ds18b20.o ./Core/Src/gpio.d ./Core/Src/gpio.o ./Core/Src/gsm.d ./Core/Src/gsm.o ./Core/Src/input_JSON.d ./Core/Src/input_JSON.o ./Core/Src/input_data.d ./Core/Src/input_data.o ./Core/Src/logic_func.d ./Core/Src/logic_func.o ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/net.d ./Core/Src/net.o ./Core/Src/rs485.d ./Core/Src/rs485.o ./Core/Src/rtc.d ./Core/Src/rtc.o ./Core/Src/spi.d ./Core/Src/spi.o ./Core/Src/spi_interface.d ./Core/Src/spi_interface.o ./Core/Src/stm32f4xx_hal_msp.d ./Core/Src/stm32f4xx_hal_msp.o ./Core/Src/stm32f4xx_hal_timebase_tim.d ./Core/Src/stm32f4xx_hal_timebase_tim.o ./Core/Src/stm32f4xx_it.d ./Core/Src/stm32f4xx_it.o ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/system_stm32f4xx.d ./Core/Src/system_stm32f4xx.o ./Core/Src/tim.d ./Core/Src/tim.o ./Core/Src/usart.d ./Core/Src/usart.o ./Core/Src/usart_ring.d ./Core/Src/usart_ring.o

.PHONY: clean-Core-2f-Src

