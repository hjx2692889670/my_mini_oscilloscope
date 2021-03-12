#include "dma.h"


void MYDMA2_Config(DMA_Channel_TypeDef* DMA_CHx,u32 cpar,u32 cmar,u16 cndtr)
{
	 DMA_InitTypeDef DMAtypeDef;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2,ENABLE);
	 DMA_StructInit(&DMAtypeDef);
	  DMA_DeInit(DMA_CHx);
	
	  DMAtypeDef.DMA_BufferSize=cndtr;
	DMAtypeDef.DMA_DIR=DMA_DIR_PeripheralDST;
	DMAtypeDef.DMA_M2M=DMA_M2M_Disable;
	DMAtypeDef.DMA_MemoryBaseAddr=cmar;
	DMAtypeDef.DMA_MemoryDataSize=DMA_MemoryDataSize_HalfWord;
	DMAtypeDef.DMA_MemoryInc=DMA_MemoryInc_Enable;
	DMAtypeDef.DMA_Mode=DMA_Mode_Circular;
	DMAtypeDef.DMA_PeripheralBaseAddr=cpar;
	DMAtypeDef.DMA_PeripheralDataSize=DMA_PeripheralDataSize_HalfWord;
	DMAtypeDef.DMA_PeripheralInc=DMA_PeripheralInc_Disable;
	DMAtypeDef.DMA_Priority=DMA_Priority_Low;
	 
	DMA_Init(DMA_CHx,&DMAtypeDef);
	 DMA_Cmd(DMA_CHx,ENABLE);
}


void MYDMA1_Config(DMA_Channel_TypeDef* DMA_CHx,u32 cpar,u32 cmar,u16 cndtr)
{
	 DMA_InitTypeDef DMAtypeDef;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	 DMA_StructInit(&DMAtypeDef);
	  DMA_DeInit(DMA_CHx);
	
	  DMAtypeDef.DMA_BufferSize=cndtr;
	DMAtypeDef.DMA_DIR=DMA_DIR_PeripheralSRC;
	DMAtypeDef.DMA_M2M=DMA_M2M_Disable;
	DMAtypeDef.DMA_MemoryBaseAddr=cmar;
	DMAtypeDef.DMA_MemoryDataSize=DMA_MemoryDataSize_HalfWord;
	DMAtypeDef.DMA_MemoryInc=DMA_MemoryInc_Enable;
	DMAtypeDef.DMA_Mode=DMA_Mode_Circular;
	DMAtypeDef.DMA_PeripheralBaseAddr=cpar;
	DMAtypeDef.DMA_PeripheralDataSize=DMA_PeripheralDataSize_HalfWord;
	DMAtypeDef.DMA_PeripheralInc=DMA_PeripheralInc_Disable;
	DMAtypeDef.DMA_Priority=DMA_Priority_High;
	 
	DMA_Init(DMA_CHx,&DMAtypeDef);
		
	 DMA_Cmd(DMA_CHx,ENABLE);
}



















