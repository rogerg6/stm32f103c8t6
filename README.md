# Introduction
- 硬件：stm32f103c8t6的核心板，通过面包板连接各种外设。
- MDK支持包 Keil.STM32F1xx_DFP.2.2.0.pack
- 标准库 STM32F10x_StdPeriph_Lib_V3.5.0
- IDE: keil uVision5
- Compiler: ARM v5.06

其余硬件原理图，芯片，外设手册等资料见百度网盘

# Contents
- start stm32启动代码
- libraries stm32片上外设库接口
- system 系统相关函数接口
- bsp 自己封装的外设接口, 更易于使用
- user 应用代码
    - gpio
    - exti 外部中断
    - timer 内部，外部时钟源定时器；OC/PWM（led呼吸灯，舵机，电机）
    - adc 模数转换
    - dma mem2mem, mem2periph搬运
    - usart 串口
    - i2c 软件模拟/硬件读取MPU6050 6轴姿态传感器数据
    - spi 软件模拟/硬件读写W25Q64 Nor Flash