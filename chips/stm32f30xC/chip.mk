#################################
# Working directories
#################################

CHIP_DIR     := $(dir $(realpath $(lastword $(MAKEFILE_LIST))))
CMSIS_DIR     = $(LIB_DIR)/CMSIS
STDPERIPH_DIR = $(LIB_DIR)/STM32F30x_StdPeriph_Driver
USBCORE_DIR   = $(LIB_DIR)/STM32_USB-FS-Device_Driver
PRINTF_DIR    = $(LIB_DIR)/printf
STARTUP_DIR   = $(CHIP_DIR)/startup
VCP_DIR       = $(CHIP_DIR)/vcp

#################################
# Include Directories
#################################

INCLUDE_DIRS += $(CHIP_DIR) \
				$(CHIP_DIR)/include \
                $(LIB_DIR) \
                $(STDPERIPH_DIR)/inc \
                $(USBCORE_DIR)/inc \
                $(CMSIS_DIR)/CM1/CoreSupport \
                $(CMSIS_DIR)/CM1/DeviceSupport/ST/STM32F30x \
                $(PRINTF_DIR) \
                $(VCP_DIR)

#################################
# Source Files
#################################

# Start up files
VPATH      := $(VPATH):$(STARTUP_DIR)
STARTUP_SRC = $(notdir $(wildcard $(STARTUP_DIR)/*.S))
LDSCRIPT    = $(STARTUP_DIR)/stm32_flash_f303_256k.ld

# Search path and source files for the ST stdperiph library
VPATH         := $(VPATH):$(STDPERIPH_DIR)/src
STDPERIPH_SRC  = $(notdir $(wildcard $(STDPERIPH_DIR)/src/*.c))

# Search path and source files for the USB libraries
VPATH       := $(VPATH):$(USBCORE_DIR)/src
USBCORE_SRC  = $(notdir $(wildcard $(USBCORE_DIR)/src/*.c))

# Search path and source files for the printf library
VPATH      := $(VPATH):$(PRINTF_DIR)
PRINTF_SRC  = $(notdir $(wildcard $(PRINTF_DIR)/*.c))

# Search path and source files for the USB VCP application source
VPATH  := $(VPATH):$(VCP_DIR)
VCP_SRC = $(notdir $(wildcard $(VCP_DIR)/*.c))

# Search path and source files for the chip peripheral CXX sources
VPATH   := $(VPATH):$(CHIP_DIR)/src
CHIP_SRC = $(notdir $(wildcard $(CHIP_DIR)/src/*.cpp))

# Search path and source files for the chip system C source
VPATH   := $(VPATH):$(CHIP_DIR)
SYS_SRC = $(CHIP_DIR)/system.c

# Append necessary C and CXX sources for this specific chip
ASOURCES   += $(STARTUP_SRC)
CSOURCES   += $(STDPERIPH_SRC) $(USBCORE_SRC) $(VCP_SRC) $(PRINTF_SRC) $(SYS_SRC)
CXXSOURCES += $(CHIP_SRC)

#################################
# Flags
#################################

# Make the binary as small as possible
C_FILE_SIZE_FLAGS   = -ffunction-sections -fdata-sections -fno-exceptions
CXX_FILE_SIZE_FLAGS = -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti

# Microcontroller flags
MCFLAGS = -mcpu=cortex-m4 -mthumb -march=armv7e-m -mfloat-abi=hard -mfpu=fpv4-sp-d16 -fsingle-precision-constant -Wdouble-promotion

# Chip-specific defines
DEFS = -DSTM32F303xC -D__CORTEX_M4 -D__FPU_PRESENT -DARM_MATH_CM4 -DWORDS_STACK_SIZE=200 -DUSE_STDPERIPH_DRIVER

CHIP_CFLAGS   = -c -std=c99   $(MCFLAGS) $(DEFS) $(C_FILE_SIZE_FLAGS)
CHIP_CXXFLAGS = -c -std=c++11 $(MCFLAGS) $(DEFS) $(CXX_FILE_SIZE_FLAGS)
CHIP_LDFLAGS  = -T$(LDSCRIPT) $(MCFLAGS) -Wl,-L$(STARTUP_DIR) -lm -lc --specs=nano.specs --specs=rdimon.specs -static -Wl,-gc-sections