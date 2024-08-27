#include "stm32f10x.h"                  // Device header
#include "PWM.h"
#include "Timer.h"

int MAX_Velo=20,Position=500,Target_Position=2500;
float KP=50,KI=0.1,KD=500,Moto;

int PID_Position(int p,int t){
	static float bias,pwm,Total_Bias,Last_Bias;
	bias=t-p;
	Total_Bias+=bias;
	pwm=KP*bias/100+KI*Total_Bias/100+KD*(bias-Last_Bias)/100;
	Last_Bias=bias;
	return pwm;
}

void Set_Position(int moto){
	Position+=moto;
	if(Position>2500)Position=2500;
	if(Position<500)Position=500;
	TIM_SetCompare2(TIM2,Position);
}

int Velo_Limit(int velo,int lim){
	int temp;
	if(velo>lim)temp=lim;
	else if(velo<-lim)temp=-lim;
	else temp=velo;
	return temp;
}


void TIM1_UP_IRQHandler(void){
	if(TIM_GetITStatus(TIM1,TIM_IT_Update)==SET){
		Moto=PID_Position(Position,Target_Position);
		Moto=Velo_Limit(Moto,MAX_Velo);
		Set_Position(Moto);
		TIM_ClearITPendingBit(TIM1,TIM_IT_Update);
	}
}



int main(void)
{
	PWM_Init();
	Timer_Init();
	while (1)
	{
		
	}
}
