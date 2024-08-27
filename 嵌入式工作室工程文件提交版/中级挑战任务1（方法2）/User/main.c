#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "MPU6050.h"
#include "math.h"
#include "Timer.h"

int16_t AX, AY, AZ, GX, GY, GZ;
double pitch_a,roll_a,yaw_a=0;

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

double get_yaw(double pitch_n,double roll_n,double yaw){
	double gy,gz;
	gy=(double)GY/16.4;
	gz=(double)GZ/16.4;
	double K_gy,K_gz;
	pitch_n=pitch_n*3.1415926/180;
	roll_n=roll_n*3.1415926/180;
	K_gy=sin(roll_n)/cos(pitch_n);
	K_gz=cos(roll_n)/cos(pitch_n);
	double dy=K_gy*gy+K_gz*gz;
	yaw+=dy*0.1;
	return yaw;
}

void TIM1_UP_IRQHandler(void){
	if(TIM_GetITStatus(TIM1,TIM_IT_Update)==SET){
		MPU6050_GetData(&AX, &AY, &AZ, &GX, &GY, &GZ);
		pitch_a=get_pitch();
		roll_a=get_roll();
		yaw_a=get_yaw(pitch_a,roll_a,yaw_a);
		OLED_ShowSignedNum(2, 1,pitch_a, 5);
		OLED_ShowSignedNum(3, 1, roll_a, 5);
		OLED_ShowSignedNum(4, 1, yaw_a, 5);
		TIM_ClearITPendingBit(TIM1,TIM_IT_Update);
	}
}

int main(void)
{
	OLED_Init();
	MPU6050_Init();
	Timer_Init();
	
	while (1)
	{
		
	}
}
