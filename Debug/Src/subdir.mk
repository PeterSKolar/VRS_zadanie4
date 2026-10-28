################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/dma.c \
../Src/gpio.c \
../Src/main.c \
../Src/stm32f3xx_it.c \
../Src/syscalls.c \
../Src/sysmem.c \
../Src/system_stm32f3xx.c \
../Src/usart.c 

OBJS += \
./Src/dma.o \
./Src/gpio.o \
./Src/main.o \
./Src/stm32f3xx_it.o \
./Src/syscalls.o \
./Src/sysmem.o \
./Src/system_stm32f3xx.o \
./Src/usart.o 

C_DEPS += \
./Src/dma.d \
./Src/gpio.d \
./Src/main.d \
./Src/stm32f3xx_it.d \
./Src/syscalls.d \
./Src/sysmem.d \
./Src/system_stm32f3xx.d \
./Src/usart.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o Src/%.su Src/%.cyclo: ../Src/%.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DEXTERNAL_CLOCK_VALUE=8000000 -DHSE_VALUE=8000000 -DHSE_STARTUP_TIMEOUT=100 -DDEBUG -DLSE_STARTUP_TIMEOUT=5000 -DLSE_VALUE=32768 -DVDD_VALUE=3300 -DLSI_VALUE=40000 -DHSI_VALUE=8000000 -DSTM32F303x8 -DUSE_FULL_LL_DRIVER -DPREFETCH_ENABLE=1 -DPOLLING=1 -c -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F3xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F3xx/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Src

clean-Src:
	-$(RM) ./Src/dma.cyclo ./Src/dma.d ./Src/dma.o ./Src/dma.su ./Src/gpio.cyclo ./Src/gpio.d ./Src/gpio.o ./Src/gpio.su ./Src/main.cyclo ./Src/main.d ./Src/main.o ./Src/main.su ./Src/stm32f3xx_it.cyclo ./Src/stm32f3xx_it.d ./Src/stm32f3xx_it.o ./Src/stm32f3xx_it.su ./Src/syscalls.cyclo ./Src/syscalls.d ./Src/syscalls.o ./Src/syscalls.su ./Src/sysmem.cyclo ./Src/sysmem.d ./Src/sysmem.o ./Src/sysmem.su ./Src/system_stm32f3xx.cyclo ./Src/system_stm32f3xx.d ./Src/system_stm32f3xx.o ./Src/system_stm32f3xx.su ./Src/usart.cyclo ./Src/usart.d ./Src/usart.o ./Src/usart.su

.PHONY: clean-Src

