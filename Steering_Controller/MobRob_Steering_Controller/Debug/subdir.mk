################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../CAN.c \
../Driving_dynamics.c \
../Globals.c \
../RC_data_recive.c \
../Steering_Servos.c \
../Ultrasonic_Collision_Avoidance.c \
../main.c \
../other_functions.c 

OBJS += \
./CAN.o \
./Driving_dynamics.o \
./Globals.o \
./RC_data_recive.o \
./Steering_Servos.o \
./Ultrasonic_Collision_Avoidance.o \
./main.o \
./other_functions.o 

C_DEPS += \
./CAN.d \
./Driving_dynamics.d \
./Globals.d \
./RC_data_recive.d \
./Steering_Servos.d \
./Ultrasonic_Collision_Avoidance.d \
./main.d \
./other_functions.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM-GCC C Compiler'
	"$(TOOLCHAIN_ROOT)/bin/arm-none-eabi-gcc" -MMD -MT "$@" -DXMC1404_F064x0200 -I"$(PROJECT_LOC)/Libraries/XMCLib/inc" -I"$(PROJECT_LOC)/Libraries/CMSIS/Include" -I"$(PROJECT_LOC)/Libraries/CMSIS/Infineon/XMC1400_series/Include" -I"$(PROJECT_LOC)" -I"$(PROJECT_LOC)/Dave/Generated" -I"$(PROJECT_LOC)/Libraries" -O0 -ffunction-sections -fdata-sections -Wall -std=gnu99 -Wa,-adhlns="$@.lst" -pipe -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d) $@" -mcpu=cortex-m0 -mthumb -g -gdwarf-2 -o "$@" "$<" 
	@echo 'Finished building: $<'
	@echo.

