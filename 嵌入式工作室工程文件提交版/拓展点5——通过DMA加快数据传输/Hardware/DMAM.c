#include "stm32f10x.h"                  // Device header

void DMAM_Init(uint32_t AddrM1,uint32_t AddrM2){
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	
	DMA_InitTypeDef DMA_InitStructure;
	DMA_InitStructure.DMA_BufferSize=1;
	DMA_InitStructure.DMA_DIR=DMA_DIR_PeripheralDST;
	DMA_InitStructure.DMA_M2M=DMA_M2M_Enable;
	DMA_InitStructure.DMA_MemoryBaseAddr=AddrM1;
	DMA_InitStructure.DMA_MemoryDataSize=DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryInc=DMA_MemoryInc_Disable;
	DMA_InitStructure.DMA_Mode=DMA_Mode_Normal;
	DMA_InitStructure.DMA_PeripheralBaseAddr=0x40000000+0x38;
	DMA_InitStructure.DMA_PeripheralDataSize=DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_PeripheralInc=DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_Priority=DMA_Priority_Medium;
	DMA_Init(DMA1_Channel1,&DMA_InitStructure);
	DMA_InitStructure.DMA_MemoryBaseAddr=AddrM2;
	DMA_InitStructure.DMA_PeripheralBaseAddr=0x40000000+0x3C;
	DMA_Init(DMA1_Channel2,&DMA_InitStructure);
	
	DMA_Cmd(DMA1_Channel1,DISABLE);
	DMA_Cmd(DMA1_Channel2,DISABLE);
}

void DMAM_Transfer1(void){
	DMA_Cmd(DMA1_Channel1,DISABLE);
	DMA_SetCurrDataCounter(DMA1_Channel1,1);
	DMA_Cmd(DMA1_Channel1,ENABLE);
	while(DMA_GetFlagStatus(DMA1_FLAG_TC1)==RESET);
	DMA_ClearFlag(DMA1_FLAG_TC1);
}

void DMAM_Transfer2(void){
	DMA_Cmd(DMA1_Channel2,DISABLE);
	DMA_SetCurrDataCounter(DMA1_Channel2,1);
	DMA_Cmd(DMA1_Channel2,ENABLE);
	while(DMA_GetFlagStatus(DMA1_FLAG_TC2)==RESET);
	DMA_ClearFlag(DMA1_FLAG_TC2);
}
