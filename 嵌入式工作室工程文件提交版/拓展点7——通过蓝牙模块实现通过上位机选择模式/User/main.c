#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "MPU6050.h"
#include "Delay.h"
#include "inv_mpu.h"
#include "PWM.h"
#include "USARTM.h"

float pitch,roll,yaw;
uint8_t res=0;

void USART1_IRQHandler(void){
	if(USART_GetITStatus(USART1,USART_IT_RXNE)==SET){
		res=USART_ReceiveData(USART1);
	}
}

int main(void)
{
	OLED_Init();
	MPU6050_Init();
	MPU6050_DMP_Init();
	PWM_Init();
	Set_Angle2(-90);
	USARTM_Init();
	while (1)
	{
		MPU6050_DMP_Get_Data(&pitch,&roll,&yaw);
		if(res==0){
			Set_Angle1(yaw);
		}else if(res==1){
			Set_Angle1(-yaw);
		}
		OLED_ShowString(4,1,"Yaw:");
		OLED_ShowSignedNum(4,7,yaw,5);
	}
}
