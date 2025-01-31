#include "stm32f10x.h"                  // Device header
//#include "usart_test.h"
//#include "sensorCounter_test.h"
//#include "rtc_test.h"
//#include "MPU6050_test.h"
//#include "W25Qxx_test.h"
//#include "rotateEncoder_test.h"
//#include "led.h"
//#include "key_test.h"
//#include "beeper.h"
//#include "photoresister.h"
//#include "iwdg_test.h"
//#include "wwdg_test.h"
//#include "adc_test.h"
//#include "flash_test.h"
//#include "dma_m2m.h"
//#include "breath_led_test.h"
//#include "motor_test.h"
//#include "servo_test.h"
//#include "freq_measure_test.h"
#include "timer.h"

int main(void) 
{
	//RTC_StandbyMode_Test();
	//MPU6050_Test();                        
	//W25Qxx_Test();
	//Rotate_Test();
	//Led_Blink();
	//Led_TubeLight(100);
	//Key_Test();
	//Beeper_Test();
	//Photoresistor_Beeper();
	//WWDG_Test();
	//ADC_SingleChannelTest();
	//ADC_MultiChannelTest();
	//Flash_Test();
	//Flash_ReadSignature();
	//DMA_M2M_Test();
	//BreathLed_Test();
	//Motor_Test();
	//Servo_Test();
	//PWMMeasure_Test();
	Timer_Test();
	
	return 0;
}


