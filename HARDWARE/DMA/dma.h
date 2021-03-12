/*-------------
DMA2  CH3  --->DAC1
---------------*/

#ifndef __DMA_H
#define	__DMA_H	   
#include "sys.h"
#include "stdio.h"
#include "DMA_STM32F10x.h"
void MYDMA1_Config(DMA_Channel_TypeDef*DMA_CHx,u32 cpar,u32 cmar,u16 cndtr);//

void MYDMA2_Config(DMA_Channel_TypeDef*DMA_CHx,u32 cpar,u32 cmar,u16 cndtr);		   
#endif




