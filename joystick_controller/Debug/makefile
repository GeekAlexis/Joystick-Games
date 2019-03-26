################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: joystick_controler.axf

# Tool invocations
joystick_controler.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU Linker'
	arm-none-eabi-gcc -nostdlib -L"/Users/alexisyang/Documents/LPCXpresso_7.3.0/workspace/lpc_board_ea_devkit_4088/Debug" -L"/Users/alexisyang/Documents/LPCXpresso_7.3.0/workspace/lpc_chip_40xx/Debug" -Xlinker -Map="joystick_controler.map" -Xlinker --gc-sections -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -mthumb -T "joystick_controler_Debug.ld" -o "joystick_controler.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(EXECUTABLES) joystick_controler.axf
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size "joystick_controler.axf"; # arm-none-eabi-objcopy -v -O binary "joystick_controler.axf" "joystick_controler.bin" ; # checksum -p LPC4088 -d "joystick_controler.bin";
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY: post-build

-include ../makefile.targets
