#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "MPU6050.h"
#include "Delay.h"
#include "inv_mpu.h"
#include "PWM.h"
#include "DMAM.h"

float pitch,roll,yaw;
uint16_t Com1,Com2;

int main(void)
{
	OLED_Init();
	MPU6050_Init();
	MPU6050_DMP_Init();
	PWM_Init();
	DMAM_Init((uint32_t)(&Com1),(uint32_t)(&Com2));
	
	while (1)
	{
		MPU6050_DMP_Get_Data(&pitch,&roll,&yaw);
		Com1=Cal_Angle((double)yaw);
		DMAM_Transfer1();
		OLED_ShowString(4,1,"Yaw:");
		OLED_ShowSignedNum(4,7,yaw,5);
	}
}
