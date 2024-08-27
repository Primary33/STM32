#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "MPU6050.h"

int16_t AX, AY, AZ, GX, GY, GZ;

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

int main(void)
{
	OLED_Init();
	MPU6050_Init();
	
	while (1)
	{
		MPU6050_GetData(&AX, &AY, &AZ, &GX, &GY, &GZ);
		OLED_ShowSignedNum(2, 1, AX, 5);
		OLED_ShowSignedNum(3, 1, AY, 5);
		OLED_ShowSignedNum(4, 1, AZ, 5);
		OLED_ShowSignedNum(2, 8, GX, 5);
		OLED_ShowSignedNum(3, 8, GY, 5);
		OLED_ShowSignedNum(4, 8, GZ, 5);
	}
}
