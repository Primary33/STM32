#include "stm32f10x.h"                  // Device header
#include "PWM.h"

int main(void)
{
	PWM_Init();
	Set_Angle(90);
	while (1)
	{
		
	}
}
