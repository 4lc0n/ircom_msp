################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
%.obj: ../%.cpp $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: MSP430 Compiler'
	"/opt/ccs/ccs1210/ccs/tools/compiler/ti-cgt-msp430_21.6.1.LTS/bin/cl430" -vmspx -Ooff --use_hw_mpy=F5 --include_path="/opt/ccs/ccs1210/ccs/ccs_base/msp430/include" --include_path="/home/h3l/Documents/Studium/Master/Sem1/PEAS2/ircom_msp/msp430_workspace/board_test" --include_path="/opt/ccs/ccs1210/ccs/tools/compiler/ti-cgt-msp430_21.6.1.LTS/include" --advice:power="all" --advice:hw_config="all" --define=__MSP430FR5948__ --define=_MPU_ENABLE -g --c11 --printf_support=minimal --diag_warning=225 --diag_wrap=off --display_error_number --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '


