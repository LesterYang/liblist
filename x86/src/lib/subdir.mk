################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/lib/list_lib.c \
../src/lib/store_list.c \
../src/lib/store_list_exte.c 

OBJS += \
./src/lib/list_lib.o \
./src/lib/store_list.o \
./src/lib/store_list_exte.o 

C_DEPS += \
./src/lib/list_lib.d \
./src/lib/store_list.d \
./src/lib/store_list_exte.d 


# Each subdirectory must supply rules for building sources it contributes
src/lib/%.o: ../src/lib/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I/mnt/hgfs/Win_7/workspace-cpp2/Liblist -I"/mnt/hgfs/Win_7/workspace-cpp2/Liblist/include" -O2 -g3 -pg -Wall -c -fPIC -fmessage-length=0  -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


