################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/ti/ccs1271/ccs/tools/compiler/ti-cgt-armllvm_3.2.2.LTS/bin/tiarmclang.exe" -c @"device.opt"  -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O2 -I"C:/Users/R5 5500U/workspace_v12/CAN_TX_hello" -I"C:/Users/R5 5500U/workspace_v12/CAN_TX_hello/Debug" -I"C:/ti/mspm0_sdk_2_02_00_05/source/third_party/CMSIS/Core/Include" -I"C:/ti/mspm0_sdk_2_02_00_05/source" -gdwarf-3 -MMD -MP -MF"$(basename $(<F)).d_raw" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

build-1991761745: ../mcan_single_message_tx.syscfg
	@echo 'Building file: "$<"'
	@echo 'Invoking: SysConfig'
	"C:/ti/sysconfig_1.21.0/sysconfig_cli.bat" --script "C:/Users/R5 5500U/workspace_v12/CAN_TX_hello/mcan_single_message_tx.syscfg" -o "." -s "C:/ti/mspm0_sdk_2_02_00_05/.metadata/product.json" --compiler ticlang
	@echo 'Finished building: "$<"'
	@echo ' '

device_linker.cmd: build-1991761745 ../mcan_single_message_tx.syscfg
device.opt: build-1991761745
device.cmd.genlibs: build-1991761745
ti_msp_dl_config.c: build-1991761745
ti_msp_dl_config.h: build-1991761745
Event.dot: build-1991761745

%.o: ./%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/ti/ccs1271/ccs/tools/compiler/ti-cgt-armllvm_3.2.2.LTS/bin/tiarmclang.exe" -c @"device.opt"  -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O2 -I"C:/Users/R5 5500U/workspace_v12/CAN_TX_hello" -I"C:/Users/R5 5500U/workspace_v12/CAN_TX_hello/Debug" -I"C:/ti/mspm0_sdk_2_02_00_05/source/third_party/CMSIS/Core/Include" -I"C:/ti/mspm0_sdk_2_02_00_05/source" -gdwarf-3 -MMD -MP -MF"$(basename $(<F)).d_raw" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

startup_mspm0g350x_ticlang.o: C:/ti/mspm0_sdk_2_02_00_05/source/ti/devices/msp/m0p/startup_system_files/ticlang/startup_mspm0g350x_ticlang.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/ti/ccs1271/ccs/tools/compiler/ti-cgt-armllvm_3.2.2.LTS/bin/tiarmclang.exe" -c @"device.opt"  -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O2 -I"C:/Users/R5 5500U/workspace_v12/CAN_TX_hello" -I"C:/Users/R5 5500U/workspace_v12/CAN_TX_hello/Debug" -I"C:/ti/mspm0_sdk_2_02_00_05/source/third_party/CMSIS/Core/Include" -I"C:/ti/mspm0_sdk_2_02_00_05/source" -gdwarf-3 -MMD -MP -MF"$(basename $(<F)).d_raw" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


