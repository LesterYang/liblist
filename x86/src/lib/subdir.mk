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
	gcc -I/mnt/hgfs/Win_7/workspace-cpp2/Liblist -I/usr/lib/gcc/i486-linux-gnu/4.4/include -I/usr/lib/gcc/i486-linux-gnu/4.4/include-fixed -I"/mnt/hgfs/Win_7/workspace-cpp2/Liblist/include" -O2 -g3 -pg -Wall -c -fmessage-length=0 -fPIC -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


