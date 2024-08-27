#include "stm32f10x.h"                  // Device header
#include "PWM.h"

int main(void)
{
	PWM_Init();
	while (1)
	{
		Circular_Rotation(-90,90);
	}
}
