################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Add inputs and outputs from these tool invocations to the build variables 
CMD_SRCS += \
../MSP_EXP432P401R_TIRTOS.cmd 

SYSCFG_SRCS += \
../LiPo_Characterization.syscfg 

C_SRCS += \
./syscfg/ti_drivers_config.c \
../callbacks.c \
../executive.c \
../main_tirtos.c \
../mem_man_tools.c \
../uart_thread.c 

GEN_FILES += \
./syscfg/ti_drivers_config.c 

GEN_MISC_DIRS += \
./syscfg/ 

C_DEPS += \
./syscfg/ti_drivers_config.d \
./callbacks.d \
./executive.d \
./main_tirtos.d \
./mem_man_tools.d \
./uart_thread.d 

OBJS += \
./syscfg/ti_drivers_config.obj \
./callbacks.obj \
./executive.obj \
./main_tirtos.obj \
./mem_man_tools.obj \
./uart_thread.obj 

GEN_MISC_FILES += \
./syscfg/ti_drivers_config.h \
./syscfg/syscfg_c.rov.xs 

GEN_MISC_DIRS__QUOTED += \
"syscfg\" 

OBJS__QUOTED += \
"syscfg\ti_drivers_config.obj" \
"callbacks.obj" \
"executive.obj" \
"main_tirtos.obj" \
"mem_man_tools.obj" \
"uart_thread.obj" 

GEN_MISC_FILES__QUOTED += \
"syscfg\ti_drivers_config.h" \
"syscfg\syscfg_c.rov.xs" 

C_DEPS__QUOTED += \
"syscfg\ti_drivers_config.d" \
"callbacks.d" \
"executive.d" \
"main_tirtos.d" \
"mem_man_tools.d" \
"uart_thread.d" 

GEN_FILES__QUOTED += \
"syscfg\ti_drivers_config.c" 

SYSCFG_SRCS__QUOTED += \
"../LiPo_Characterization.syscfg" 

C_SRCS__QUOTED += \
"./syscfg/ti_drivers_config.c" \
"../callbacks.c" \
"../executive.c" \
"../main_tirtos.c" \
"../mem_man_tools.c" \
"../uart_thread.c" 


