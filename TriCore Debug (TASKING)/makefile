################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include McmcanFd/subdir.mk
-include Libraries/iLLD/TC3xx/Tricore/_PinMap/TC37x/subdir.mk
-include Libraries/iLLD/TC3xx/Tricore/_Lib/DataHandling/subdir.mk
-include Libraries/iLLD/TC3xx/Tricore/_Impl/subdir.mk
-include Libraries/iLLD/TC3xx/Tricore/Stm/Std/subdir.mk
-include Libraries/iLLD/TC3xx/Tricore/Src/Std/subdir.mk
-include Libraries/iLLD/TC3xx/Tricore/Scu/Std/subdir.mk
-include Libraries/iLLD/TC3xx/Tricore/Port/Std/subdir.mk
-include Libraries/iLLD/TC3xx/Tricore/Pms/Std/subdir.mk
-include Libraries/iLLD/TC3xx/Tricore/Mtu/Std/subdir.mk
-include Libraries/iLLD/TC3xx/Tricore/Gtm/Std/subdir.mk
-include Libraries/iLLD/TC3xx/Tricore/Cpu/Trap/subdir.mk
-include Libraries/iLLD/TC3xx/Tricore/Cpu/Std/subdir.mk
-include Libraries/iLLD/TC3xx/Tricore/Cpu/Irq/subdir.mk
-include Libraries/iLLD/TC3xx/Tricore/Can/Std/subdir.mk
-include Libraries/iLLD/TC3xx/Tricore/Can/Can/subdir.mk
-include Libraries/iLLD/TC3xx/Tricore/Asclin/Std/subdir.mk
-include Libraries/iLLD/TC3xx/Tricore/Asclin/Asc/subdir.mk
-include Libraries/Infra/Ssw/TC3xx/Tricore/subdir.mk
-include Libraries/Infra/Platform/Tricore/Compilers/subdir.mk
-include Configurations/Debug/subdir.mk
-include Configurations/subdir.mk
-include subdir.mk
ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C++M_DEPS)),)
-include $(C++M_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(CCM_DEPS)),)
-include $(CCM_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(CXXM_DEPS)),)
-include $(CXXM_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := HA_Project3_MotorECU
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_SIZE += \
HA_Project3_MotorECU.siz \


# All Target
all: main-build

# Main-build Target
main-build: HA_Project3_MotorECU.elf secondary-outputs

# Tool invocations
HA_Project3_MotorECU.elf: $(OBJS) makefile $(OPTIONAL_TOOL_DEPS) .HA_Project3_MotorECU.elf.opt
	@echo 'Building target: $@'
	@echo 'Invoking: TASKING Linker'
	cctc -f .HA_Project3_MotorECU.elf.opt
	@echo 'Finished building target: $@'
	@echo ' '

.HA_Project3_MotorECU.elf.opt:
	@argfile .HA_Project3_MotorECU.elf.opt --lsl-file="../Lcf_Tasking_Tricore_Tc.lsl" -Wl-Oc -Wl-OL -Wl-Ot -Wl-Ox -Wl-Oy -Wl--map-file="HA_Project3_MotorECU.map" -Wl-mc -Wl-mf -Wl-mi -Wl-mk -Wl-ml -Wl-mm -Wl-md -Wl-mr -Wl-mu --no-warnings= -Wl--error-limit=42 --fp-model=3 -lrt --lsl-core=vtc --exceptions --strict --anachronisms --force-c++ -Ctc37x -o"HA_Project3_MotorECU.elf" -Wl-o"HA_Project3_MotorECU.hex:IHEX" $(OBJS)

.PHONY: .HA_Project3_MotorECU.elf.opt

HA_Project3_MotorECU.siz: HA_Project3_MotorECU.elf makefile $(OPTIONAL_TOOL_DEPS)
	@echo 'Invoking: Print Size'
	elfsize  "HA_Project3_MotorECU.elf"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) HA_Project3_MotorECU.siz HA_Project3_MotorECU.elf
	-@echo ' '

secondary-outputs: $(SECONDARY_SIZE)

.PHONY: all clean dependents main-build

-include ../makefile.targets
