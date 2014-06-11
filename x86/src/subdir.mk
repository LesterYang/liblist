################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/close_list.c \
../src/list.c \
../src/open_list.c \
../src/print_list.c \
../src/search.c \
../src/sort.c 

OBJS += \
./src/close_list.o \
./src/list.o \
./src/open_list.o \
./src/print_list.o \
./src/search.o \
./src/sort.o 

C_DEPS += \
./src/close_list.d \
./src/list.d \
./src/open_list.d \
./src/print_list.d \
./src/search.d \
./src/sort.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I/mnt/hgfs/Win_7/workspace-cpp2/Liblist -I/usr/lib/gcc/i486-linux-gnu/4.4/include -I/usr/lib/gcc/i486-linux-gnu/4.4/include-fixed -I"/mnt/hgfs/Win_7/workspace-cpp2/Liblist/include" -O2 -g3 -pg -Wall -c -fmessage-length=0 -fPIC -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


