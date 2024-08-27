#include "stm32f10x.h"                  // Device header
#include "I2CM.h"
#include "Delay.h"

uint8_t MPU6050_Write(uint8_t addr,uint8_t reg,uint8_t len,uint8_t *buf){
	unsigned char i;
	addr=addr<<1;
	I2CM_Start();
	I2CM_SendByte(addr);
	I2CM_SendByte(reg);
	
	for(i=0;i<len;i++){
		I2CM_SendByte(*buf++);
	}
	I2CM_Stop();
	return 0;
}

uint8_t  MPU6050_Read(uint8_t addr,uint8_t reg,uint8_t len,uint8_t *buf){
	unsigned char i;
	addr=addr<<1;                     
	I2CM_Start();              
	I2CM_SendByte(addr);   
	I2CM_SendByte(reg);    
	I2CM_Start();    
	I2CM_SendByte(addr+1);
	for(i=0;i<len-1;i++){
		*buf++=I2CM_ReceiveByte(0);
	}
	*buf=I2CM_ReceiveByte(1);
	I2CM_Stop();
	return 0;
}

void MPU6050_Write_Reg(uint8_t reg,uint8_t dat){
	MPU6050_Write(0x68,reg,1,&dat);
}

uint8_t MPU6050_Read_Reg(uint8_t reg){
	uint8_t dat;
	MPU6050_Read(0x68,reg,1,&dat);
	return dat;
}

uint8_t MPU_Set_Gyro_Fsr(uint8_t fsr){
	MPU6050_Write_Reg(0x1B,fsr<<3);
	return 0;
}

uint8_t MPU_Set_Accel_Fsr(uint8_t fsr){
	MPU6050_Write_Reg(0x1C,fsr<<3);
	return 0;
}

uint8_t MPU_Set_LPF(uint16_t lpf){
	uint8_t data=0;
	if(lpf>=188)data=1;
	else if(lpf>=98)data=2;
	else if(lpf>=42)data=3;
	else if(lpf>=20)data=4;
	else if(lpf>=10)data=5;
	else data=6; 
	MPU6050_Write_Reg(0x1A,data); 
	return 0;
}

uint8_t MPU_Set_Rate(uint16_t rate){
	uint8_t data;
	if(rate>1000)rate=1000;
	if(rate<4)rate=4;
	data=1000/rate-1;
	MPU6050_Write_Reg(0x19,data);
	return MPU_Set_LPF(rate/2);
}

void MPU6050_Init(void){
	uint8_t res;
	I2CM_Init();
	MPU6050_Write_Reg(0x6B,0x80);
	Delay_ms(100);
	MPU6050_Write_Reg(0x6B,0x00);
	MPU_Set_Gyro_Fsr(3);
	MPU_Set_Accel_Fsr(0);
	MPU_Set_Rate(200);
	MPU6050_Write_Reg(0x38,0x00);
	MPU6050_Write_Reg(0x6A,0x00);
	MPU6050_Write_Reg(0x23,0x00);
	MPU6050_Write_Reg(0x37,0x80);
	res=MPU6050_Read_Reg(0x75);
	if(res==0x68){
		MPU6050_Write_Reg(0x6B,0x01);
		MPU6050_Write_Reg(0x6C,0x00);
		MPU_Set_Rate(100);
	}
}
