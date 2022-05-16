################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/periphery_io/Src/data_process.c \
../Core/periphery_io/Src/temperature_sensors.c 

OBJS += \
./Core/periphery_io/Src/data_process.o \
./Core/periphery_io/Src/temperature_sensors.o 

C_DEPS += \
./Core/periphery_io/Src/data_process.d \
./Core/periphery_io/Src/temperature_sensors.d 


# Each subdirectory must supply rules for building sources it contributes
Core/periphery_io/Src/%.o: ../Core/periphery_io/Src/%.c Core/periphery_io/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DDEBUG -DSTM32F407xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/LwIP/src/include/netif/ppp -I../Middlewares/Third_Party/LwIP/src/include/lwip/priv -I../Middlewares/Third_Party/LwIP/src/include/compat/stdc -I../Middlewares/Third_Party/LwIP/src/include/lwip/prot -I../Middlewares/Third_Party/LwIP/src/include/lwip/apps -I../Middlewares/Third_Party/LwIP/src/include/compat/posix/sys -I../Middlewares/Third_Party/LwIP/system/arch -I../Middlewares/Third_Party/LwIP/src/include -I../LWIP/App -I../Middlewares/Third_Party/LwIP/src/include/compat/posix -I../LWIP/Target -I../Middlewares/Third_Party/LwIP/src/include/lwip -I../Middlewares/Third_Party/LwIP/src/include/compat/posix/net -I../Middlewares/Third_Party/LwIP/src/include/compat/posix/arpa -I../Middlewares/Third_Party/LwIP/system -I../Middlewares/Third_Party/LwIP/src/include/netif -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-periphery_io-2f-Src

clean-Core-2f-periphery_io-2f-Src:
	-$(RM) ./Core/periphery_io/Src/data_process.d ./Core/periphery_io/Src/data_process.o ./Core/periphery_io/Src/temperature_sensors.d ./Core/periphery_io/Src/temperature_sensors.o

.PHONY: clean-Core-2f-periphery_io-2f-Src

