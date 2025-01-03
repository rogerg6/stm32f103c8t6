/**
 * @brief 外设功能测试函数
 *
 */
 
 
#ifndef __MY_DRIVERS_H
#define __MY_DRIVERS_H

#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "gpio.h"
#include "OLED.h"

void Led_Blink(void);
void Led_TubeLight(uint32_t ms);
void Beeper_Test(void);
void Button_Test(void);
void Photoresistor_Beeper(void);
void OLED_Test(void);
void SensorCounter_Test(void);


#endif

