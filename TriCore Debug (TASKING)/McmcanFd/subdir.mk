################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
"../McmcanFd/McmcanFd.c" 

COMPILED_SRCS += \
"McmcanFd/McmcanFd.src" 

C_DEPS += \
"./McmcanFd/McmcanFd.d" 

OBJS += \
"McmcanFd/McmcanFd.o" 


# Each subdirectory must supply rules for building sources it contributes
"McmcanFd/McmcanFd.src":"../McmcanFd/McmcanFd.c" "McmcanFd/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2012 -D__CPU__=tc37x "-fC:/Users/asdds/AURIX-v1.10.28-workspace/HA_Project3_MotorECU/TriCore Debug (TASKING)/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Wc-w508 -Ctc37x -Y0 -N0 -Z0 -o "$@" "$<"
"McmcanFd/McmcanFd.o":"McmcanFd/McmcanFd.src" "McmcanFd/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"

clean: clean-McmcanFd

clean-McmcanFd:
	-$(RM) ./McmcanFd/McmcanFd.d ./McmcanFd/McmcanFd.o ./McmcanFd/McmcanFd.src

.PHONY: clean-McmcanFd

