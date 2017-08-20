################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Plans/Plan.cpp \
../Plans/PlnA.cpp 

OBJS += \
./Plans/Plan.o \
./Plans/PlnA.o 

CPP_DEPS += \
./Plans/Plan.d \
./Plans/PlnA.d 


# Each subdirectory must supply rules for building sources it contributes
Plans/%.o: ../Plans/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


