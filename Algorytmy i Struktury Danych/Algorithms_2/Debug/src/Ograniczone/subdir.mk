################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/Ograniczone/mon2.c 

OBJS += \
./src/Ograniczone/mon2.o 

C_DEPS += \
./src/Ograniczone/mon2.d 


# Each subdirectory must supply rules for building sources it contributes
src/Ograniczone/%.o: ../src/Ograniczone/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


