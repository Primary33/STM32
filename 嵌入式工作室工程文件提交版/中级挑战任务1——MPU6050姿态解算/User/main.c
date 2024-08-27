#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "MPU6050.h"
#include "Delay.h"
#include "inv_mpu.h"

float pitch,roll,yaw;

int main(void)
{
	OLED_Init();
	MPU6050_Init();
	MPU6050_DMP_Init();
	
	while (1)
	{
		MPU6050_DMP_Get_Data(&pitch,&roll,&yaw);
		OLED_ShowString(2,1,"Pitch:");
		OLED_ShowSignedNum(2,7,pitch,5);
		OLED_ShowString(3,1,"Roll:");
		OLED_ShowSignedNum(3,7,roll,5);
		OLED_ShowString(4,1,"Yaw:");
		OLED_ShowSignedNum(4,7,yaw,5);
	}
}
