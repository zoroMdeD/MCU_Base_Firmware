################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/fatfs/Src/ccsbcs.c \
../Core/fatfs/Src/ff.c \
../Core/fatfs/Src/sd_cmd.c \
../Core/fatfs/Src/sd_main.c \
../Core/fatfs/Src/spi_sd.c 

OBJS += \
./Core/fatfs/Src/ccsbcs.o \
./Core/fatfs/Src/ff.o \
./Core/fatfs/Src/sd_cmd.o \
./Core/fatfs/Src/sd_main.o \
./Core/fatfs/Src/spi_sd.o 

C_DEPS += \
./Core/fatfs/Src/ccsbcs.d \
./Core/fatfs/Src/ff.d \
./Core/fatfs/Src/sd_cmd.d \
./Core/fatfs/Src/sd_main.d \
./Core/fatfs/Src/spi_sd.d 


# Each subdirectory must supply rules for building sources it contributes
Core/fatfs/Src/%.o: ../Core/fatfs/Src/%.c Core/fatfs/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DDEBUG -DSTM32F407xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/LwIP/src/include/netif/ppp -I../Middlewares/Third_Party/LwIP/src/include/lwip/priv -I../Middlewares/Third_Party/LwIP/src/include/compat/stdc -I../Middlewares/Third_Party/LwIP/src/include/lwip/prot -I../Middlewares/Third_Party/LwIP/src/include/lwip/apps -I../Middlewares/Third_Party/LwIP/src/include/compat/posix/sys -I../Middlewares/Third_Party/LwIP/system/arch -I../Middlewares/Third_Party/LwIP/src/include -I../LWIP/App -I../Middlewares/Third_Party/LwIP/src/include/compat/posix -I../LWIP/Target -I../Middlewares/Third_Party/LwIP/src/include/lwip -I../Middlewares/Third_Party/LwIP/src/include/compat/posix/net -I../Middlewares/Third_Party/LwIP/src/include/compat/posix/arpa -I../Middlewares/Third_Party/LwIP/system -I../Middlewares/Third_Party/LwIP/src/include/netif -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-fatfs-2f-Src

clean-Core-2f-fatfs-2f-Src:
	-$(RM) ./Core/fatfs/Src/ccsbcs.d ./Core/fatfs/Src/ccsbcs.o ./Core/fatfs/Src/ff.d ./Core/fatfs/Src/ff.o ./Core/fatfs/Src/sd_cmd.d ./Core/fatfs/Src/sd_cmd.o ./Core/fatfs/Src/sd_main.d ./Core/fatfs/Src/sd_main.o ./Core/fatfs/Src/spi_sd.d ./Core/fatfs/Src/spi_sd.o

.PHONY: clean-Core-2f-fatfs-2f-Src

