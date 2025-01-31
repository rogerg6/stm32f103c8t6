#include "rotateEncoder.h"
#include "OLED.h"

void Rotate_Test(void)
{
	RotateEncoder_Init();
	OLED_Init();
	
	OLED_ShowString(1, 1, "cnt = ");
	
	while (1) {
		OLED_ShowSignedNum(1, 7, RotateEncoder_GetCount(), 5);
	}
}
