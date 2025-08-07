# Introduction
江协stm32入门教程代码
- 硬件：stm32f103c8t6的核心板，通过面包板连接各种外设。
- MDK支持包 Keil.STM32F1xx_DFP.2.2.0.pack
- 标准库 STM32F10x_StdPeriph_Lib_V3.5.0
- IDE: keil uVision5
- Compiler: ARM v5.06

其余硬件原理图，芯片，外设手册等资料见百度网盘

# Build
- 支持keil构建
- 支持makefile构建(armcc.mk), 支持openocd下载
- 支持makefile(gcc.mk), 开源gnu工具链构建
- 支持clangd解析：compiledb make -f armcc.mk

# Contents
### 根据目录组织
- start stm32启动代码
- libraries stm32片上外设库接口
- system cpu系统功能代码
- hardware cpu外设功能代码
- unittest 功能代码的测试代码
- user 应用代码
- rtthread-nano rtthread板级支持

### 根据模块组织
| 模块    | 内容     |
|----------------|-------------|
| GPIO   |`hardware/gpio`、`LED hardware/led`、`按键 hardware/button`、`蜂鸣器 hardware/beeper`、`光敏电阻 hardware/photoresister`     |
| EXTI(外部中断)   |  `对射式红外传感器 hardware/sensorCounter`、`旋转编码器 hardware/rotateEncoder`   |
| Timer   | `基本定时器中断 system/timer`、`外部时钟源定时器中断 unittest/external_timer`、`PWM system/pwm.h、呼吸灯 unittest/breath_led_test、电机 unittest/motor_test、舵机 unittest/servo_test`、`IC输入捕获system/ic、测周法测频率和占空比unittest/freq_measure_test、编码器测速unittest/encoder_speed_measure`    |
| ADC   | `单/多通道ADC hardware/adc`、`ADC多通道单次扫描+DMA单次传输模式 hardware/adc_dma1`、 `【推荐】ADC多通道连续扫描+DMA连续传输模式 hardware/adc_dma2`  |
| DMA   | `内存间数据搬运 system/dma_m2m`   |
| USART   | `hardware/usart`     |
| I2C   | `软件模拟I2C(hardware/i2c_sim)`、`软硬件I2C(hardware/MPU6050)`、`hardware/OLED`  |
| SPI   |  `软硬实现SPI(hardware/spi)`、`hardware/W25Qxx`   |
| PWR(电源管理)   |`修改系统主频 unittest/change_freq`、`睡眠模式 unitest/usart_test`、`停机模式 unittest/sensorCounter_test`、`待机模式 unittest/rtc_test`   |
| WatchDog(看门狗) | `独立看门狗 unittest/iwdg_test`、`窗口看门狗 unittest/wwdg_test` |
|stm32内部FLASH | `读写内部FLASH system/flash, unittest/flash_test`、`Flash容量，芯片ID unittest/flash_test`|
|CAN| `can总线hardware/can.c, unittest/can_*_test.c` |

