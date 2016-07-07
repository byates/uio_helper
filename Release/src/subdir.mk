################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/uio_find_devices.c \
../src/uio_find_devices_byname.c \
../src/uio_free.c \
../src/uio_get_all_info.c \
../src/uio_get_device_attributes.c \
../src/uio_get_event_count.c \
../src/uio_get_mem_addr.c \
../src/uio_get_mem_size.c \
../src/uio_get_name.c \
../src/uio_get_version.c \
../src/uio_line_from_file.c \
../src/uio_mmap.c \
../src/uio_munmap.c \
../src/uio_num_from_filename.c \
../src/uio_single_mmap.c \
../src/uio_single_munmap.c 

OBJS += \
./src/uio_find_devices.o \
./src/uio_find_devices_byname.o \
./src/uio_free.o \
./src/uio_get_all_info.o \
./src/uio_get_device_attributes.o \
./src/uio_get_event_count.o \
./src/uio_get_mem_addr.o \
./src/uio_get_mem_size.o \
./src/uio_get_name.o \
./src/uio_get_version.o \
./src/uio_line_from_file.o \
./src/uio_mmap.o \
./src/uio_munmap.o \
./src/uio_num_from_filename.o \
./src/uio_single_mmap.o \
./src/uio_single_munmap.o 

C_DEPS += \
./src/uio_find_devices.d \
./src/uio_find_devices_byname.d \
./src/uio_free.d \
./src/uio_get_all_info.d \
./src/uio_get_device_attributes.d \
./src/uio_get_event_count.d \
./src/uio_get_mem_addr.d \
./src/uio_get_mem_size.d \
./src/uio_get_name.d \
./src/uio_get_version.d \
./src/uio_line_from_file.d \
./src/uio_mmap.d \
./src/uio_munmap.d \
./src/uio_num_from_filename.d \
./src/uio_single_mmap.d \
./src/uio_single_munmap.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-linux-gnueabihf-gcc -O3 -Wall -c -fmessage-length=0 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


