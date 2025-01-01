# Introduction
硬件：stm32f103c8t6的核心板，通过面包板连接各种外设。
MDK支持包 Keil.STM32F1xx_DFP.2.2.0.pack
标准库 STM32F10x_StdPeriph_Lib_V3.5.0
IDE: keil uVision5
Compiler: ARM v5.06

其余硬件原理图，芯片，外设手册等资料见百度网盘

# Contents
- start stm32启动代码
- libraries stm32片上外设库接口
- system 系统相关函数接口
- bsp 自己封装的外设接口, 更易于使用
- user 应用代码