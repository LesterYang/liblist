################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/lib/free2.c \
../src/lib/list_lib.c \
../src/lib/sort_listdata.c 

OBJS += \
./src/lib/free2.o \
./src/lib/list_lib.o \
./src/lib/sort_listdata.o 

C_DEPS += \
./src/lib/free2.d \
./src/lib/list_lib.d \
./src/lib/sort_listdata.d 


# Each subdirectory must supply rules for building sources it contributes
src/lib/%.o: ../src/lib/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	/opt/freescale/usr/local/gcc-4.4.4-glibc-2.11.1-multilib-1.0/arm-fsl-linux-gnueabi/bin/arm-fsl-linux-gnueabi-gcc -I"/mnt/hgfs/Win_7/workspace-cpp2/Liblist" -I"/mnt/hgfs/Win_7/workspace-cpp2/Liblist/include" -O2 -Wall -pipe -c -fPIC -fmessage-length=0 -march=armv7-a -mfpu=neon -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


