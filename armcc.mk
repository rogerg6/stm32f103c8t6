
BUILD_DIR = build
TARGET = $(BUILD_DIR)/prog.axf

# C sources
C_SOURCES =  \
start/core_cm3.c \
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
unittest/can_test.c              \
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
start/startup_stm32f10x_md.s \


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
PREFIX = D:/.install/keil5/keil5_MDK/ARM/ARMCC/bin/
CC = $(PREFIX)armcc
AS = $(PREFIX)armasm
LINK = $(PREFIX)armlink
AR = $(PREFIX)armar
 
# interwork: 允许ARM/Thumb指令混合调用	兼容旧版ARM代码和Thumb高效指令
# split_sections: 函数级代码分节（每函数独立section）	关键优化：链接器可移除未调用函数，显著减少Flash占用（适合库文件多的项目）
CFLAGS = --c99 -c --cpu Cortex-M3 -D__MICROLIB -g -O0 --apcs=interwork --split_sections \
-DSTM32F10X_MD -DUSE_STDPERIPH_DRIVER \

# --pd: 汇编预处理定义（等同于C的-D）
# xref: 生成交叉引用表, 生成符号交叉引用表，用于追踪汇编代码中所有符号，是实现代码可靠性和空间优化的基石工具
ASFLAGS = --cpu Cortex-M3 -g --apcs=interwork --pd "__MICROLIB SETA 1" \
--pd "STM32F10X_MD SETA 1" --xref  

# strict: 启用严格模式（警告视为错误）
# scatter: 指定分散加载文件
LDFLAGS = --cpu Cortex-M3 \
--library_type=microlib --strict --scatter "proj.sct" 


# default action: build all
all: $(TARGET)

#######################################
# build the application
#######################################
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

$(BUILD_DIR)/%.o: %.c
	@echo $(TOPDIR)
	$(CC) $(CFLAGS) $(C_INCLUDES) $< -o $@

$(BUILD_DIR)/%.o: %.s
	$(AS) $(ASFLAGS) $< -o $@

$(TARGET): $(BUILD_DIR) $(OBJECTS)
	$(LINK) $(LDFLAGS) $(OBJECTS) -o $@

$(BUILD_DIR):
	mkdir -p $@		

download:
	@openocd.exe -f stlink-dap.cfg -f stm32f1x.cfg -c "program $(TARGET) verify reset exit"

clean:
	rm -rf $(BUILD_DIR)
  