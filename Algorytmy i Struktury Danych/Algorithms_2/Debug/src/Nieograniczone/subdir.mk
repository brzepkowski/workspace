################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/Nieograniczone/mon1.c 

OBJS += \
./src/Nieograniczone/mon1.o 

C_DEPS += \
./src/Nieograniczone/mon1.d 


# Each subdirectory must supply rules for building sources it contributes
src/Nieograniczone/%.o: ../src/Nieograniczone/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


