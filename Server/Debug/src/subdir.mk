################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/CommandHandlerData.cpp \
../src/OnProcessImageSrv.cpp \
../src/Server.cpp \
../src/ServerCommandHandler.cpp \
../src/main.cpp 

OBJS += \
./src/CommandHandlerData.o \
./src/OnProcessImageSrv.o \
./src/Server.o \
./src/ServerCommandHandler.o \
./src/main.o 

CPP_DEPS += \
./src/CommandHandlerData.d \
./src/OnProcessImageSrv.d \
./src/Server.d \
./src/ServerCommandHandler.d \
./src/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	arm-linux-gnueabihf-g++ -std=c++0x -I"/home/mark/workspace/ImageCapture/ClientServerCommon/src" -I/home/mark/workspace/libjpeg-turbo-2.0.4 -I/usr/arm-linux-gnueabihf/include/c++/5.4.0 -I"/home/mark/workspace/ImageCapture/Utils/src" -I"/home/mark/workspace/ImageCapture/ImageCapture/src" -I"/home/mark/workspace/ImageCapture/ImageCapture/src/ImageProcessing" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


