#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "MPU6050.h"
#include "Delay.h"
#include "inv_mpu.h"
#include "PWM.h"

float pitch,roll,yaw;

int main(void)
{
	OLED_Init();
	MPU6050_Init();
	MPU6050_DMP_Init();
	PWM_Init();
	Set_Angle2(-90);
	
	while (1)
	{
		MPU6050_DMP_Get_Data(&pitch,&roll,&yaw);
		Set_Angle1(-yaw);
		OLED_ShowString(4,1,"Yaw:");
		OLED_ShowSignedNum(4,7,yaw,5);
	}
}
