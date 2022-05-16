################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Middlewares/Third_Party/LwIP/src/netif/ppp/subdir.mk
-include Middlewares/Third_Party/LwIP/src/netif/subdir.mk
-include Middlewares/Third_Party/LwIP/src/core/ipv6/subdir.mk
-include Middlewares/Third_Party/LwIP/src/core/ipv4/subdir.mk
-include Middlewares/Third_Party/LwIP/src/core/subdir.mk
-include Middlewares/Third_Party/LwIP/src/apps/mqtt/subdir.mk
-include Middlewares/Third_Party/LwIP/src/api/subdir.mk
-include LWIP/Target/subdir.mk
-include LWIP/App/subdir.mk
-include Drivers/STM32F4xx_HAL_Driver/Src/subdir.mk
-include Core/rs485/Src/subdir.mk
-include Core/periphery_io/Src/subdir.mk
-include Core/lcd_interface/Src/subdir.mk
-include Core/gsm/Src/subdir.mk
-include Core/fatfs/Src/subdir.mk
-include Core/eth/Src/subdir.mk
-include Core/dwt/Src/subdir.mk
-include Core/Startup/subdir.mk
-include Core/Src/subdir.mk
-include Core/JSON/Src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
MCU_Base_Firmware_CubeMX.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
MCU_Base_Firmware_CubeMX.list \

OBJCOPY_BIN += \
MCU_Base_Firmware_CubeMX.bin \


# All Target
all: MCU_Base_Firmware_CubeMX.elf secondary-outputs

# Tool invocations
MCU_Base_Firmware_CubeMX.elf: $(OBJS) $(USER_OBJS) D:\YandexDisk\YandexDisk\BasicControlAutomated\MCU_Base_Firmware_CubeMX\MCU_Base_Firmware\MCU_Base_Firmware_CubeMX\STM32F407VGTX_FLASH.ld
	arm-none-eabi-gcc -o "MCU_Base_Firmware_CubeMX.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m4 -T"D:\YandexDisk\YandexDisk\BasicControlAutomated\MCU_Base_Firmware_CubeMX\MCU_Base_Firmware\MCU_Base_Firmware_CubeMX\STM32F407VGTX_FLASH.ld" --specs=nosys.specs -Wl,-Map="MCU_Base_Firmware_CubeMX.map" -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -u _printf_float -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

MCU_Base_Firmware_CubeMX.list: $(EXECUTABLES)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "MCU_Base_Firmware_CubeMX.list"
	@echo 'Finished building: $@'
	@echo ' '

MCU_Base_Firmware_CubeMX.bin: $(EXECUTABLES)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "MCU_Base_Firmware_CubeMX.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) *
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_BIN)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
