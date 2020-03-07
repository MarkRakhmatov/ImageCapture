################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Buffer.cpp \
../src/VideoDevice.cpp \
../src/main.cpp 

OBJS += \
./src/Buffer.o \
./src/VideoDevice.o \
./src/main.o 

CPP_DEPS += \
./src/Buffer.d \
./src/VideoDevice.d \
./src/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	arm-linux-gnueabihf-g++ -std=c++1y -I/usr/arm-linux-gnueabihf/include/c++/5/ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


