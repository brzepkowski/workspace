################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/lista4-daneTestowe/sub-daneTestowe/subdir.mk
-include src/lista4-daneTestowe/inwersje-daneTestowe/subdir.mk
-include src/lista4-daneTestowe/gog-daneTestowe/subdir.mk
-include src/lista4-daneTestowe/cykl-daneTestowe/subdir.mk
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
all: Algorithms_4

# Tool invocations
Algorithms_4: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C Linker'
	gcc  -o "Algorithms_4" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(EXECUTABLES) Algorithms_4
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
