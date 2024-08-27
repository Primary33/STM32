#include "stm32f10x.h"                  // Device header
#include "Delay.h"

void I2CM_W_SCL(uint8_t BitVal){
	GPIO_WriteBit(GPIOB,GPIO_Pin_10,(BitAction)BitVal);
	Delay_us(10);
}

void I2CM_W_SDA(uint8_t BitVal){
	GPIO_WriteBit(GPIOB,GPIO_Pin_11,(BitAction)BitVal);
	Delay_us(10);
}

uint8_t I2CM_R_SDA(void){
	uint8_t BitVal;
	BitVal=GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11);
	Delay_us(10);
	return BitVal;
}

void I2CM_Init(){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_SetBits(GPIOB,GPIO_Pin_10 | GPIO_Pin_11);
}

void I2CM_Start(void){
	I2CM_W_SDA(1);
	I2CM_W_SCL(1);
	I2CM_W_SDA(0);
	I2CM_W_SCL(0);
}

void I2CM_Stop(void){
	I2CM_W_SDA(0);
	I2CM_W_SCL(1);
	I2CM_W_SDA(1);
}

uint8_t I2CM_ReceiveAck(void){
	uint8_t Ack=0;
	I2CM_W_SDA(1);
	I2CM_W_SCL(1);
	Ack=I2CM_R_SDA();
	I2CM_W_SCL(0);
	return Ack;
}

void I2CM_SendAck(uint8_t AckBit){
	I2CM_W_SDA(AckBit);
	I2CM_W_SCL(1);
	I2CM_W_SCL(0);
}

void I2CM_SendByte(uint8_t byte){
	uint8_t i;
	for(i=0;i<8;i++){
		I2CM_W_SDA(byte & (0x80 >> i));
		I2CM_W_SCL(1);
		I2CM_W_SCL(0);
	}
	I2CM_ReceiveAck();
}

uint8_t I2CM_ReceiveByte(uint8_t Ack){
	uint8_t i,byte=0x00;
	I2CM_W_SDA(1);
	for(i=0;i<8;i++){
		I2CM_W_SCL(1);
		if(I2CM_R_SDA()==1){
			byte|=(0x80 >> i);
		}
		I2CM_W_SCL(0);
	}
	I2CM_SendAck(Ack);
	return byte;
}
