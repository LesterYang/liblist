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
	g++ -I/mnt/hgfs/Win_7/workspace-cpp2/Liblist -I/usr/include/c++/4.4 -I/usr/include/c++/4.4/backward -I/usr/include/c++/4.4/i486-linux-gnu -I/usr/lib/gcc/i486-linux-gnu/4.4/include -I/usr/lib/gcc/i486-linux-gnu/4.4/include-fixed -I"/mnt/hgfs/Win_7/workspace-cpp2/Liblist/include" -O2 -g3 -pg -Wall -c -fPIC -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


