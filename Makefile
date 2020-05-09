PROJECT_NAME := sopwith

GNU_INSTALL_ROOT := ~/gcc-arm-none-eabi-8-2019-q3-update
GNU_VERSION := 4.9.3
GNU_PREFIX := arm-none-eabi

STM32CUBEF4_ROOT := ~/STMicroelectronics/STM32CubeF4

SEMIHOSTING ?= "OFF"
$(info Semihosting is $(SEMIHOSTING))

ifeq ("$(SEMIHOSTING)", "ON")
OUTPUT_FILENAME := sopwith_sh
else
OUTPUT_FILENAME := sopwith
endif

$(info Output filename $(OUTPUT_FILENAME))

MAKEFILE_NAME := $(MAKEFILE_LIST)
MAKEFILE_DIR := $(dir $(MAKEFILE_NAME) ) 

MK := mkdir
RM := rm -rf

#echo suspend
ifeq ("$(VERBOSE)","1")
NO_ECHO := 
else
NO_ECHO := @
endif

# Toolchain commands
CC              := '$(GNU_INSTALL_ROOT)/bin/$(GNU_PREFIX)-gcc'
AS              := '$(GNU_INSTALL_ROOT)/bin/$(GNU_PREFIX)-as'
AR              := '$(GNU_INSTALL_ROOT)/bin/$(GNU_PREFIX)-ar' -r
LD              := '$(GNU_INSTALL_ROOT)/bin/$(GNU_PREFIX)-ld'
NM              := '$(GNU_INSTALL_ROOT)/bin/$(GNU_PREFIX)-nm'
OBJDUMP         := '$(GNU_INSTALL_ROOT)/bin/$(GNU_PREFIX)-objdump'
OBJCOPY         := '$(GNU_INSTALL_ROOT)/bin/$(GNU_PREFIX)-objcopy'
SIZE            := '$(GNU_INSTALL_ROOT)/bin/$(GNU_PREFIX)-size'

# Function for removing duplicates in a list
remduplicates = $(strip $(if $1,$(firstword $1) $(call remduplicates,$(filter-out $(firstword $1),$1))))

# Sopwith source files
SOPWITH_SOURCE_FILES = \
$(abspath sopwithsource/swmain.c) \
$(abspath sopwithsource/swhist.c) \
$(abspath sopwithsource/swinit.c) \
$(abspath sopwithsource/swmisc.c) \
$(abspath sopwithsource/swtitle.c) \
$(abspath sopwithsource/swobject.c) \
$(abspath sopwithsource/swutil.c) \
$(abspath sopwithsource/swsound.c) \
$(abspath sopwithsource/swcomm.c) \
$(abspath sopwithsource/swauto.c) \
$(abspath sopwithsource/swcollsn.c) \
$(abspath sopwithsource/swgrpha.c) \
$(abspath sopwithsource/swdisp.c) \
$(abspath sopwithsource/swgames.c) \
$(abspath sopwithsource/swground.c) \
$(abspath sopwithsource/swend.c) \
$(abspath sopwithsource/swplanes.c) \
$(abspath sopwithsource/swmove.c) \
$(abspath sopwithsource/swsymbol.c)
#$(abspath sopwithsource/bmblib.c)
#$(abspath sopwithsource/swmultio.c) # Multplayer mode not supported
#$(abspath sopwithsource/swasynio.c) # Async mode not supported 
#$(abspath sopwithsource/swnetio.c)
#$(abspath sopwithsource/_INTC.C) \

# Sources
C_SOURCE_FILES += \
$(abspath stm3241g/src/system_stm32f4xx.c) \
$(abspath stm3241g/src/stm32f4xx_it.c) \
$(abspath stm3241g/src/stm32f4xx_hal_msp.c) \
$(abspath stm3241g/src/swaudio_hal.c) \
$(abspath stm3241g/src/swtimer_hal.c) \
$(abspath stm3241g/src/swbuttons_hal.c) \
$(abspath stm3241g/src/swdisplay_hal.c) \
$(abspath stm3241g/src/swrand_hal.c) \
$(abspath stm3241g/src/main.c) \
$(abspath $(STM32CUBEF4_ROOT)/Drivers/BSP/Components/stmpe811/stmpe811.c) \
$(abspath $(STM32CUBEF4_ROOT)/Drivers/BSP/Components/ili9325/ili9325.c) \
$(abspath $(STM32CUBEF4_ROOT)/Drivers/BSP/Components/cs43l22/cs43l22.c) \
$(abspath $(STM32CUBEF4_ROOT)/Drivers/BSP/STM324xG_EVAL/stm324xg_eval.c) \
$(abspath $(STM32CUBEF4_ROOT)/Drivers/BSP/STM324xG_EVAL/stm324xg_eval_io.c) \
$(abspath $(STM32CUBEF4_ROOT)/Drivers/BSP/STM324xG_EVAL/stm324xg_eval_lcd.c) \
$(abspath $(STM32CUBEF4_ROOT)/Drivers/BSP/STM324xG_EVAL/stm324xg_eval_audio.c) \
$(abspath $(STM32CUBEF4_ROOT)/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal.c) \
$(abspath $(STM32CUBEF4_ROOT)/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim.c) \
$(abspath $(STM32CUBEF4_ROOT)/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim_ex.c) \
$(abspath $(STM32CUBEF4_ROOT)/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_i2c.c) \
$(abspath $(STM32CUBEF4_ROOT)/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_i2s.c) \
$(abspath $(STM32CUBEF4_ROOT)/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_i2s_ex.c) \
$(abspath $(STM32CUBEF4_ROOT)/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma.c) \
$(abspath $(STM32CUBEF4_ROOT)/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc_ex.c) \
$(abspath $(STM32CUBEF4_ROOT)/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_sram.c) \
$(abspath $(STM32CUBEF4_ROOT)/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc.c) \
$(abspath $(STM32CUBEF4_ROOT)/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rng.c) \
$(abspath $(STM32CUBEF4_ROOT)/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_uart.c) \
$(abspath $(STM32CUBEF4_ROOT)/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cortex.c) \
$(abspath $(STM32CUBEF4_ROOT)/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_gpio.c) \
$(abspath $(STM32CUBEF4_ROOT)/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_fsmc.c) \
$(SOPWITH_SOURCE_FILES) \

ifneq ("$(SEMIHOSTING)", "ON")
C_SOURCE_FILES += \
$(abspath stm3241g/src/syscalls.c) 
endif

# Startup script
ASM_SOURCE_FILES  = $(abspath stm3241g/src/startup_stm32f417xx.s)

# Sopwith include paths
SOPWITH_INC_PATHS = -I$(abspath ./sopwithsource)

# Includes
INC_PATHS = -I$(abspath .) \
-I$(abspath ./stm3241g/include) \
-I$(abspath $(STM32CUBEF4_ROOT)/Drivers/CMSIS/Device/ST/STM32F4xx/Include) \
-I$(abspath $(STM32CUBEF4_ROOT)/Drivers/CMSIS/Include) \
-I$(abspath $(STM32CUBEF4_ROOT)/Drivers/BSP/Components/stmpe800) \
-I$(abspath $(STM32CUBEF4_ROOT)/Drivers/BSP/Components/cs43l22) \
-I$(abspath $(STM32CUBEF4_ROOT)/Drivers/STM32F4xx_HAL_Driver/Inc) \
-I$(abspath $(STM32CUBEF4_ROOT)/Drivers/BSP/STM324xG_EVAL) \
$(SOPWITH_INC_PATHS) \

OBJECT_DIRECTORY = _build
LISTING_DIRECTORY = $(OBJECT_DIRECTORY)
OUTPUT_BINARY_DIRECTORY = $(OBJECT_DIRECTORY)

# Sorting removes duplicates
BUILD_DIRECTORIES := $(sort $(OBJECT_DIRECTORY) $(OUTPUT_BINARY_DIRECTORY) $(LISTING_DIRECTORY) )

CFLAGS += -DSTM32F4
CFLAGS += -DSTM32F417IG
CFLAGS += -DSTM32F417xx
CFLAGS += -mcpu=cortex-m4
CFLAGS += -mthumb 
CFLAGS += -mabi=aapcs
CFLAGS += --std=gnu99
CFLAGS += -Wall -Werror -O0
CFLAGS += -mfloat-abi=soft
CFLAGS += -ffunction-sections 
CFLAGS += -fdata-sections 
CFLAGS += -fno-strict-aliasing
CFLAGS += -fno-builtin --short-enums
CFLAGS += -g3
CFLAGS += -Wno-unused-variable
CFLAGS += -Wno-unused-function

# keep every function in separate section. This will allow linker to dump unused functions
LDFLAGS += -Xlinker -Map=$(LISTING_DIRECTORY)/$(OUTPUT_FILENAME).map
LDFLAGS += -mthumb -mabi=aapcs -T$(LINKER_SCRIPT)
LDFLAGS += -mcpu=cortex-m4
# let linker to dump unused sections
LDFLAGS += -Wl,--gc-sections

ifeq ("$(SEMIHOSTING)", "ON")
# Semihosting
LDFLAGS += -specs=rdimon.specs -lc -lrdimon -specs=nosys.specs
#LDFLAGS +=  -specs=nano.specs
else
# use newlib in nano version
#LDFLAGS +=  -specs=nano.specs
LDFLAGS +=  -lc -lnosys
endif

# Assembler flags
ASMFLAGS += -x assembler-with-cpp
#default target - first one defined
default: sopwith

#building all targets
all: clean
	$(NO_ECHO)$(MAKE) -f $(MAKEFILE_NAME) -C $(MAKEFILE_DIR) -e cleanobj
	$(NO_ECHO)$(MAKE) -f $(MAKEFILE_NAME) -C $(MAKEFILE_DIR) -e sopwith
	$(NO_ECHO)$(MAKE) -f $(MAKEFILE_NAME) -C $(MAKEFILE_DIR) -e sopwith_sh

#target for printing all targets
help:
	@echo following targets are available:
	@echo 	sopwith
	@echo   flash
	@echo   debug
	@echo   sopwith_sh - for Semihosting
	@echo   flash_sh - for Semihosting
	@echo   debug_sh - for Semihosting


C_SOURCE_FILE_NAMES = $(notdir $(C_SOURCE_FILES))
C_PATHS = $(call remduplicates, $(dir $(C_SOURCE_FILES) ) )
C_OBJECTS = $(addprefix $(OBJECT_DIRECTORY)/, $(C_SOURCE_FILE_NAMES:.c=.o) )

ASM_SOURCE_FILE_NAMES = $(notdir $(ASM_SOURCE_FILES))
ASM_PATHS = $(call remduplicates, $(dir $(ASM_SOURCE_FILES) ))
ASM_OBJECTS = $(addprefix $(OBJECT_DIRECTORY)/, $(ASM_SOURCE_FILE_NAMES:.s=.o) )

vpath %.c $(C_PATHS)
vpath %.s $(ASM_PATHS)

OBJECTS = $(C_OBJECTS) $(ASM_OBJECTS)

sopwith: LINKER_SCRIPT=stm3241g/linker/STM32F417IGHx_FLASH.ld
sopwith: $(BUILD_DIRECTORIES) $(OBJECTS)
	@echo Linking target: $(OUTPUT_FILENAME).out
	$(NO_ECHO)$(CC) $(LDFLAGS) $(OBJECTS) $(LIBS) -o $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out
	$(NO_ECHO)$(MAKE) -f $(MAKEFILE_NAME) -C $(MAKEFILE_DIR) -e finalize
	
sopwith_sh: 
	$(NO_ECHO)$(MAKE) -f $(MAKEFILE_NAME) -C $(MAKEFILE_DIR) -e sopwith SEMIHOSTING=ON

## Create build directories
$(BUILD_DIRECTORIES):
	echo $(MAKEFILE_NAME)
	$(MK) $@

# Create objects from C SRC files
$(OBJECT_DIRECTORY)/%.o: %.c
	@echo Compiling file: $(notdir $<)
	$(NO_ECHO)$(CC) $(CFLAGS) $(INC_PATHS) -c -o $@ $<

# Assemble files
$(OBJECT_DIRECTORY)/%.o: %.s
	@echo Compiling file: $(notdir $<)
	$(NO_ECHO)$(CC) $(ASMFLAGS) $(INC_PATHS) -c -o $@ $<


# Link
$(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out: $(BUILD_DIRECTORIES) $(OBJECTS)
	@echo Linking target: $(OUTPUT_FILENAME).out
	$(NO_ECHO)$(CC) $(LDFLAGS) $(OBJECTS) $(LIBS) -o $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out


## Create binary .bin file from the .out file
$(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).bin: $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out
	@echo Preparing: $(OUTPUT_FILENAME).bin
	$(NO_ECHO)$(OBJCOPY) -O binary $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).bin

## Create binary .hex file from the .out file
$(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).hex: $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out
	@echo Preparing: $(OUTPUT_FILENAME).hex
	$(NO_ECHO)$(OBJCOPY) -O ihex $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).hex

finalize: genbin genhex echosize

genbin:
	@echo Preparing: $(OUTPUT_FILENAME).bin
	$(NO_ECHO)$(OBJCOPY) -O binary $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).bin

## Create binary .hex file from the .out file
genhex: 
	@echo Preparing: $(OUTPUT_FILENAME).hex
	$(NO_ECHO)$(OBJCOPY) -O ihex $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).hex

echosize:
	-@echo ''
	$(NO_ECHO)$(SIZE) $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out
	-@echo ''

clean:
	$(RM) $(BUILD_DIRECTORIES)

cleanobj:
	$(RM) $(BUILD_DIRECTORIES)/*.o

flash: $(MAKECMDGOALS)
	@echo Flashing: $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).bin
	st-flash write $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).bin 0x8000000
	st-flash reset

flash_sh: 
	$(NO_ECHO)$(MAKE) -f $(MAKEFILE_NAME) -C $(MAKEFILE_DIR) -e flash SEMIHOSTING=ON
	
debug:
	mstmdebug.sh -e _build/sopwith.out -g 3333
	
debug_sh:
	mstmdebug.sh -e _build/sopwith_sh.out -g 3333 -w
