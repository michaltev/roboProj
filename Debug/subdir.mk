################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../ConfigurationManager.cpp \
../Grid.cpp \
../LocalizationManager.cpp \
../Map.cpp \
../MapDisplay.cpp \
../Navigator.cpp \
../Node.cpp \
../Particle.cpp \
../PathPlanner.cpp \
../Robot.cpp \
../WaypointsManager.cpp \
../main.cpp 

OBJS += \
./ConfigurationManager.o \
./Grid.o \
./LocalizationManager.o \
./Map.o \
./MapDisplay.o \
./Navigator.o \
./Node.o \
./Particle.o \
./PathPlanner.o \
./Robot.o \
./WaypointsManager.o \
./main.o 

CPP_DEPS += \
./ConfigurationManager.d \
./Grid.d \
./LocalizationManager.d \
./Map.d \
./MapDisplay.d \
./Navigator.d \
./Node.d \
./Particle.d \
./PathPlanner.d \
./Robot.d \
./WaypointsManager.d \
./main.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


