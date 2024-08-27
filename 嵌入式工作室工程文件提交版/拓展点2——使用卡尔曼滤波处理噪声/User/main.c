#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "MPU6050.h"
#include "Delay.h"
#include "inv_mpu.h"
#include "math.h"
#include "Timer.h"

int16_t AX, AY, AZ, GX, GY, GZ;
double Roll_Kalman=0,Pitch_Kalman=0;

void MPU6050_GetData(int16_t *Accx,int16_t *Accy,int16_t *Accz,
						int16_t *Gyrox,int16_t *Gyroy,int16_t *Gyroz)
{
	uint8_t datah,datal;
	datah=MPU6050_Read_Reg(0x3B);
	datal=MPU6050_Read_Reg(0x3C);
	*Accx=(datah << 8) | datal;
	
	datah=MPU6050_Read_Reg(0x3D);
	datal=MPU6050_Read_Reg(0x3E);
	*Accy=(datah << 8) | datal;
	
	datah=MPU6050_Read_Reg(0x3F);
	datal=MPU6050_Read_Reg(0x40);
	*Accz=(datah << 8) | datal;
	
	datah=MPU6050_Read_Reg(0x43);
	datal=MPU6050_Read_Reg(0x44);
	*Gyrox=(datah << 8) | datal;
	
	datah=MPU6050_Read_Reg(0x45);
	datal=MPU6050_Read_Reg(0x46);
	*Gyroy=(datah << 8) | datal;
	
	datah=MPU6050_Read_Reg(0x47);
	datal=MPU6050_Read_Reg(0x48);
	*Gyroz=(datah << 8) | datal;
}

double get_pitch(void){
	double pitch;
	double ax,ay,az;
	ax=(double)AX/100;
	ay=(double)AY/100;
	az=(double)AZ/100;
	double t1_pitch=ax;
	double t2_pitch=ay*ay+az*az;
	t2_pitch=sqrt(t2_pitch);
	pitch=-atan(t1_pitch/t2_pitch);
	pitch=pitch*180/3.1415926;
	return pitch;
}

double get_roll(void){
	double roll;
	double ay,az;
	ay=(double)AY/100;
	az=(double)AZ/100;
	roll=atan(ay/az);
	roll=roll*180/3.1415926;
	return roll;
}

void Pitch_Cal(double angle,double gyro)
{
	static double Q_angle=0.001,Q_gyro=0.003,dt=0.01,R_angle=0.01;
	static double Q_bias;
	static double K0, K1;
	static double PP[2][2]={{1,0},{0,1}};
	Pitch_Kalman+=(gyro-Q_bias)*dt;
	
	PP[0][0]=PP[0][0]+Q_angle-(PP[0][1]+PP[1][0])*dt;
	PP[0][1]=PP[0][1]-PP[1][1]*dt;
	PP[1][0]=PP[1][0]-PP[1][1]*dt;
	PP[1][1]=PP[1][1]+Q_gyro;
	
	K0=PP[0][0]/(PP[0][0]+R_angle);
	K1=PP[1][0]/(PP[0][0]+R_angle);
	
	Pitch_Kalman=Pitch_Kalman+K0*(angle-Pitch_Kalman);
	Q_bias=Q_bias+K1*(angle-Pitch_Kalman);
	
	PP[0][0]=PP[0][0]-K0*PP[0][0];
	PP[0][1]=PP[0][1]-K0*PP[0][1];
	PP[1][0]=PP[1][0]-K1*PP[0][0];
	PP[1][1]=PP[1][1]-K1*PP[0][1];
}

void Roll_Cal(double angle,double gyro)
{
	static double Q_angle=0.001,Q_gyro=0.003,dt=0.01,R_angle=0.01;
	static double Q_bias;
	static double K0, K1;
	static double PP[2][2]={{1,0},{0,1}};
	Roll_Kalman+=(gyro-Q_bias)*dt;
	
	PP[0][0]=PP[0][0]+Q_angle-(PP[0][1]+PP[1][0])*dt;
	PP[0][1]=PP[0][1]-PP[1][1]*dt;
	PP[1][0]=PP[1][0]-PP[1][1]*dt;
	PP[1][1]=PP[1][1]+Q_gyro;
	
	K0=PP[0][0]/(PP[0][0]+R_angle);
	K1=PP[1][0]/(PP[0][0]+R_angle);
	
	Roll_Kalman=Roll_Kalman+K0*(angle-Roll_Kalman);
	Q_bias=Q_bias+K1*(angle-Roll_Kalman);
	PP[0][0]=PP[0][0]-K0*PP[0][0];
	PP[0][1]=PP[0][1]-K0*PP[0][1];
	PP[1][0]=PP[1][0]-K1*PP[0][0];
	PP[1][1]=PP[1][1]-K1*PP[0][1];
}

void TIM1_UP_IRQHandler(void){
	if(TIM_GetITStatus(TIM1,TIM_IT_Update)==SET){
		MPU6050_GetData(&AX,&AY,&AZ,&GX,&GY,&GZ);
		double pitch_n=get_pitch();
		double gy=(double)GY/16.4;
		Pitch_Cal(pitch_n,gy);
		double roll_n=get_roll();
		double gx=(double)GX/16.4;
		Roll_Cal(roll_n,gx);
		OLED_ShowString(1,1,"Pitch:");
		OLED_ShowSignedNum(1,7,Pitch_Kalman,5);
		OLED_ShowString(2,1,"Roll:");
		OLED_ShowSignedNum(2,7,Roll_Kalman,5);
		TIM_ClearITPendingBit(TIM1,TIM_IT_Update);
	}
}


int main(void)
{
	OLED_Init();
	MPU6050_Init();
	Timer_Init();
	MPU6050_GetData(&AX,&AY,&AZ,&GX,&GY,&GZ);
	Pitch_Kalman=get_pitch();
	Roll_Kalman=get_roll();
	
	while (1)
	{
		
	}
}
