################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/close_list.c \
../src/liblist_version.c \
../src/list.c \
../src/list_subdir.c \
../src/open_list.c \
../src/print_list.c \
../src/search_POI.c \
../src/search_file.c \
../src/sort.c \
../src/store_list.c \
../src/store_list_exte.c 

OBJS += \
./src/close_list.o \
./src/liblist_version.o \
./src/list.o \
./src/list_subdir.o \
./src/open_list.o \
./src/print_list.o \
./src/search_POI.o \
./src/search_file.o \
./src/sort.o \
./src/store_list.o \
./src/store_list_exte.o 

C_DEPS += \
./src/close_list.d \
./src/liblist_version.d \
./src/list.d \
./src/list_subdir.d \
./src/open_list.d \
./src/print_list.d \
./src/search_POI.d \
./src/search_file.d \
./src/sort.d \
./src/store_list.d \
./src/store_list_exte.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I/mnt/hgfs/Win_7/workspace-cpp2/Liblist -I"/mnt/hgfs/Win_7/workspace-cpp2/Liblist/include" -O2 -g3 -pg -Wall -c -fPIC -fmessage-length=0  -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


