################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/Ograniczone/Posortowane\ listy/mon2.c 

OBJS += \
./src/Ograniczone/Posortowane\ listy/mon2.o 

C_DEPS += \
./src/Ograniczone/Posortowane\ listy/mon2.d 


# Each subdirectory must supply rules for building sources it contributes
src/Ograniczone/Posortowane\ listy/mon2.o: ../src/Ograniczone/Posortowane\ listy/mon2.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"src/Ograniczone/Posortowane listy/mon2.d" -MT"src/Ograniczone/Posortowane\ listy/mon2.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


