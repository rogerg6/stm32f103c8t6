
BUILD_DIR = build
TARGET = prog

# C sources
C_SOURCES =  \
start/core_cm3_gcc.c \
start/system_stm32f10x.c \
libraries/misc.c             \
libraries/stm32f10x_gpio.c   \
libraries/stm32f10x_rcc.c    \
libraries/stm32f10x_rtc.c    \
libraries/stm32f10x_usart.c  \
libraries/stm32f10x_exti.c   \
system/Delay.c   \
hardware/gpio.c                  \
hardware/led.c                   \
hardware/usart.c                 \
user/stm32f10x_it_rtt.c              \
rtthread-nano/bsp/stm32f103/main.c                      \
rtthread-nano/bsp/stm32f103/board.c                      \
rtthread-nano/src/clock.c     \
rtthread-nano/src/components.c \
rtthread-nano/src/cpu.c       \
rtthread-nano/src/device.c    \
rtthread-nano/src/idle.c      \
rtthread-nano/src/ipc.c       \
rtthread-nano/src/irq.c       \
rtthread-nano/src/kservice.c  \
rtthread-nano/src/mem.c       \
rtthread-nano/src/memheap.c   \
rtthread-nano/src/mempool.c   \
rtthread-nano/src/object.c    \
rtthread-nano/src/scheduler.c \
rtthread-nano/src/signal.c    \
rtthread-nano/src/slab.c      \
rtthread-nano/src/thread.c    \
rtthread-nano/src/timer.c     \
rtthread-nano/libcpu/arm/cortex-m3/cpuport.c \

# ASM sources
ASM_SOURCES =  \
start/gcc_ride7/startup_stm32f10x_md_rtt.s \
rtthread-nano/libcpu/arm/cortex-m3/context_gcc.S \


# C includes
C_INCLUDES = \
-Ilibraries\
-Istart\
-Isystem\
-Ihardware\
-Iunittest\
-Iuser \
-Irtthread-nano/include \
-Irtthread-nano/bsp/stm32f103 \

#######################################
# binaries
#######################################
PREFIX = arm-none-eabi-
CC = $(PREFIX)gcc
AS = $(PREFIX)as
LINK = $(PREFIX)ld
CP = $(PREFIX)objcopy
SZ = $(PREFIX)size
HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S
 
CPU = -mcpu=cortex-m3
MCU = $(CPU) -mthumb $(FPU) $(FLOAT-ABI)

CFLAGS = $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections
CFLAGS += -DSTM32F10X_MD -DUSE_STDPERIPH_DRIVER
ASFLAGS = $(MCU) $(AS_DEFS) $(AS_INCLUDES) $(OPT) -Wall 
 
LDSCRIPT = start/gcc_ride7/stm32f10x_flash_extsram.ld

LIBS = -lc
LIBDIR = 
#LDFLAGS = $(MCU) -specs=nano.specs -T$(LDSCRIPT) $(LIBDIR) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections
LDFLAGS = $(MCU) -T$(LDSCRIPT) $(LIBDIR) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections


# default action: build all
all: $(BUILD_DIR) $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin


#######################################
# build the application
#######################################
# list of objects
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))
# list of ASM program objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
OBJECTS := $(OBJECTS:.S=.o)
vpath %.s $(sort $(dir $(ASM_SOURCES)))
vpath %.S $(sort $(dir $(ASM_SOURCES)))
 
$(BUILD_DIR)/%.o: %.c 
	$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@ 
$(BUILD_DIR)/%.o: %.s 
	$(AS) -c $(ASFLAGS) $< -o $@
$(BUILD_DIR)/%.o: %.S
	$(CC) $(C_INCLUDES) -c $(CFLAGS) $< -o $@
 
$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) $(LDSCRIPT)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(SZ) $@
 
$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(HEX) $< $@
	
$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(BIN) $< $@	
	
$(BUILD_DIR):
	mkdir $@	

download:
# openocd.exe -f stlink.cfg -f stm32f1x.cfg -c "program $(BUILD_DIR)/$(TARGET).bin 0x08000000 verify reset exit"
	@openocd.exe -f stlink.cfg -f stm32f1x.cfg -c "program $(BUILD_DIR)/$(TARGET).hex verify reset exit"

clean:
	rm -rf $(BUILD_DIR)
  
-include $(wildcard $(BUILD_DIR)/*.d)