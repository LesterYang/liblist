################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/lib/list_lib.c \
../src/lib/list_lib_vector.c \
../src/lib/store_list.c 

OBJS += \
./src/lib/list_lib.o \
./src/lib/list_lib_vector.o \
./src/lib/store_list.o 

C_DEPS += \
./src/lib/list_lib.d \
./src/lib/list_lib_vector.d \
./src/lib/store_list.d 


# Each subdirectory must supply rules for building sources it contributes
src/lib/%.o: ../src/lib/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	/opt/freescale/usr/local/gcc-4.4.4-glibc-2.11.1-multilib-1.0/arm-fsl-linux-gnueabi/bin/arm-fsl-linux-gnueabi-gcc -I"/mnt/hgfs/Win_7/workspace-cpp2/Liblist" -I/Space/ltib2/ltib/rootfs/usr/include -I/Space/ltib2/ltib/rootfs/include -I"/mnt/hgfs/Win_7/workspace-cpp2/Liblist/include" -O2 -Wall -c -fmessage-length=0 -fPIC -mcpu=cortex-a8 -mfpu=neon -mfloat-abi=softfp -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


