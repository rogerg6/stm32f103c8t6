
BUILD_DIR = build
TARGET = prog

# C sources
C_SOURCES =  \
start/core_cm3_gcc.c \
start/system_stm32f10x.c \
libraries/misc.c             \
libraries/stm32f10x_adc.c    \
libraries/stm32f10x_bkp.c    \
libraries/stm32f10x_can.c    \
libraries/stm32f10x_cec.c    \
libraries/stm32f10x_crc.c    \
libraries/stm32f10x_dac.c    \
libraries/stm32f10x_dbgmcu.c \
libraries/stm32f10x_dma.c    \
libraries/stm32f10x_exti.c   \
libraries/stm32f10x_flash.c  \
libraries/stm32f10x_fsmc.c   \
libraries/stm32f10x_gpio.c   \
libraries/stm32f10x_i2c.c    \
libraries/stm32f10x_iwdg.c   \
libraries/stm32f10x_pwr.c    \
libraries/stm32f10x_rcc.c    \
libraries/stm32f10x_rtc.c    \
libraries/stm32f10x_sdio.c   \
libraries/stm32f10x_spi.c    \
libraries/stm32f10x_tim.c    \
libraries/stm32f10x_usart.c  \
libraries/stm32f10x_wwdg.c   \
system/Delay.c   \
system/bkp.c     \
system/dma_m2m.c \
system/flash.c   \
system/ic.c      \
system/pwm.c     \
system/rtc.c     \
system/timer.c   \
hardware/MPU6050.c               \
hardware/OLED.c                  \
hardware/W25Qxx.c                \
hardware/adc.c                   \
hardware/adc_dma1.c              \
hardware/beeper.c                \
hardware/can.c                   \
hardware/gpio.c                  \
hardware/i2c_sim.c               \
hardware/key.c                   \
hardware/led.c                   \
hardware/photoresister.c         \
hardware/rotateEncoder.c         \
hardware/sensorCounter.c         \
hardware/spi.c                   \
hardware/usart.c                 \
unittest/MPU6050_test.c          \
unittest/W25Qxx_test.c           \
unittest/adc_test.c              \
unittest/breath_led_test.c       \
unittest/can_transmit_test.c              \
unittest/can_recieve_test.c              \
unittest/change_freq.c           \
unittest/encoder_speed_measure.c \
unittest/external_timer.c        \
unittest/flash_test.c            \
unittest/freq_measure_test.c     \
unittest/iwdg_test.c             \
unittest/key_test.c              \
unittest/motor_test.c            \
unittest/rotateEncoder_test.c    \
unittest/rtc_test.c              \
unittest/sensorCounter_test.c    \
unittest/servo_test.c            \
unittest/usart_test.c            \
unittest/wwdg_test.c             \
user/main.c                      \
user/stm32f10x_it.c              \

# ASM sources
ASM_SOURCES =  \
start/gcc_ride7/startup_stm32f10x_md.s \


# C includes
C_INCLUDES = \
-Ilibraries\
-Istart\
-Isystem\
-Ihardware\
-Iunittest\
-Iuser

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
vpath %.s $(sort $(dir $(ASM_SOURCES)))
 
$(BUILD_DIR)/%.o: %.c 
	$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@
$(BUILD_DIR)/%.o: %.s 
	$(AS) -c $(ASFLAGS) $< -o $@
 
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
	@openocd.exe -f stlink.cfg -f stm32f1x.cfg -c "program $(BUILD_DIR)/$(TARGET).hex verify reset exit"

clean:
	rm -rf $(BUILD_DIR)
  
-include $(wildcard $(BUILD_DIR)/*.d)