################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
%.obj: ../%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/ti/ccs1110/ccs/tools/compiler/ti-cgt-arm_20.2.7.LTS/bin/armcl" -mv7M3 --code_state=16 --float_support=vfplib -me --include_path="C:/Users/lutfi yildirim/workspace_v11/AOM12864A0-1.54WW-ANO_v8" --include_path="C:/ti/simplelink_cc13x0_sdk_4_20_02_07/source/ti/posix/ccs" --include_path="C:/ti/ccs1110/ccs/tools/compiler/ti-cgt-arm_20.2.7.LTS/include" --define=BOARD_DISPLAY_USE_UART_ANSI=1 --define=BOARD_DISPLAY_USE_LCD=1 --define=DeviceFamily_CC13X0 -g --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


