################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/lista4-daneTestowe/gog-daneTestowe/gog.c 

OBJS += \
./src/lista4-daneTestowe/gog-daneTestowe/gog.o 

C_DEPS += \
./src/lista4-daneTestowe/gog-daneTestowe/gog.d 


# Each subdirectory must supply rules for building sources it contributes
src/lista4-daneTestowe/gog-daneTestowe/%.o: ../src/lista4-daneTestowe/gog-daneTestowe/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


