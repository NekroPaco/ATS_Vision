################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../fileops.cpp \
../imgTransf.cpp \
../main.cpp \
../neuralnet.cpp 

OBJS += \
./fileops.o \
./imgTransf.o \
./main.o \
./neuralnet.o 

CPP_DEPS += \
./fileops.d \
./imgTransf.d \
./main.d \
./neuralnet.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/include/opencv -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


