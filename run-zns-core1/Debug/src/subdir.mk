################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
LD_SRCS += \
../src/lscript.ld 

C_SRCS += \
../src/address_translation.c \
../src/data_buffer.c \
../src/ftl_config.c \
../src/main.c \
../src/nsc_driver.c \
../src/nvme_io_cmd_completion.c \
../src/request_allocation.c \
../src/request_ipc.c \
../src/request_schedule.c \
../src/request_transform.c 

OBJS += \
./src/address_translation.o \
./src/data_buffer.o \
./src/ftl_config.o \
./src/main.o \
./src/nsc_driver.o \
./src/nvme_io_cmd_completion.o \
./src/request_allocation.o \
./src/request_ipc.o \
./src/request_schedule.o \
./src/request_transform.o 

C_DEPS += \
./src/address_translation.d \
./src/data_buffer.d \
./src/ftl_config.d \
./src/main.d \
./src/nsc_driver.d \
./src/nvme_io_cmd_completion.d \
./src/request_allocation.d \
./src/request_ipc.d \
./src/request_schedule.d \
./src/request_transform.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM v8 gcc compiler'
	aarch64-none-elf-gcc -Wall -O0 -g3 -c -fmessage-length=0 -MT"$@" -I../../run-zns-core1_bsp/psu_cortexa53_1/include -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


