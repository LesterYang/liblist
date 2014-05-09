################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/close_list.c \
../src/free2.c \
../src/liblist_version.c \
../src/list.c \
../src/list_subdir.c \
../src/open_list.c \
../src/print_list.c \
../src/search_POI.c \
../src/search_file.c \
../src/sort_alph.c \
../src/sort_exte.c \
../src/sort_list.c \
../src/sort_listdata.c \
../src/sort_size.c \
../src/sort_time.c \
../src/store_list.c \
../src/store_list_exte.c 

OBJS += \
./src/close_list.o \
./src/free2.o \
./src/liblist_version.o \
./src/list.o \
./src/list_subdir.o \
./src/open_list.o \
./src/print_list.o \
./src/search_POI.o \
./src/search_file.o \
./src/sort_alph.o \
./src/sort_exte.o \
./src/sort_list.o \
./src/sort_listdata.o \
./src/sort_size.o \
./src/sort_time.o \
./src/store_list.o \
./src/store_list_exte.o 

C_DEPS += \
./src/close_list.d \
./src/free2.d \
./src/liblist_version.d \
./src/list.d \
./src/list_subdir.d \
./src/open_list.d \
./src/print_list.d \
./src/search_POI.d \
./src/search_file.d \
./src/sort_alph.d \
./src/sort_exte.d \
./src/sort_list.d \
./src/sort_listdata.d \
./src/sort_size.d \
./src/sort_time.d \
./src/store_list.d \
./src/store_list_exte.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	/opt/freescale/usr/local/gcc-4.4.4-glibc-2.11.1-multilib-1.0/arm-fsl-linux-gnueabi/bin/arm-fsl-linux-gnueabi-gcc -I"/mnt/hgfs/Win_7/workspace-cpp2/Liblist/include" -I/Space/ltib2/ltib/rootfs/usr/include -I/Space/ltib2/ltib/rootfs/include -O3 -g3 -pg -Wall -c -fPIC -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


