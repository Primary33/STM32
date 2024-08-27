#include "stm32f10x.h"                  // Device header
#include "Delay.h"

#define RCC_APB2ENR *((unsigned int*)(0x40021000+0x18))
#define GPIOB_CRH *((unsigned int*)(0x40010C00+0x04))
#define GPIOB_ODR *((unsigned int*)(0x40010C00+0x0C))
#define GPIOB_IDR *((unsigned int*)(0x40010C00+0x08))

void I2CM_W_SCL(uint8_t BitVal){
	if(BitVal==0){
		GPIOB_ODR&=~(1<<10);
	}else{
		GPIOB_ODR|=(1<<10);
	}
	Delay_us(10);
}

void I2CM_W_SDA(uint8_t BitVal){
	if(BitVal==0){
		GPIOB_ODR&=~(1<<11);
	}else{
		GPIOB_ODR|=(1<<11);
	}
	Delay_us(10);
}

uint8_t I2CM_R_SDA(void){
	uint8_t BitVal;
	BitVal=(GPIOB_IDR&(1<<11))>>11;
	Delay_us(10);
	return BitVal;
}

void I2CM_Init(void){
	RCC_APB2ENR|=1<<3;
	
	GPIOB_CRH&=0xFFFF00FF;
	GPIOB_CRH|=0x00007700;
	
	GPIOB_ODR=(1<<10)|(1<<11);
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
