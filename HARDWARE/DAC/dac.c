/*-------------------------------------------------------------------------------------
  mini示波器
	2021.2.18 bySDS
	
	 TMR+DMA+DAC实现
	 
	DAC  使能PA4
	
---------------------------------------------------------------------------------------*/
#include "dac.h"
void Dac1_Init(void)
{
	DAC_InitTypeDef DACtypeDef;
	GPIO_PortClock(GPIOA,ENABLE); //端口A 使能
	GPIO_PinConfigure(GPIOA,4,GPIO_IN_ANALOG,GPIO_MODE_INPUT);  //端口5的模式配置
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC,ENABLE);
	
	DAC_DeInit(); //
	DAC_StructInit(&DACtypeDef);//
	DACtypeDef.DAC_LFSRUnmask_TriangleAmplitude=DAC_LFSRUnmask_Bit0;
	DACtypeDef.DAC_OutputBuffer=DAC_OutputBuffer_Disable;
	DACtypeDef.DAC_Trigger=DAC_Trigger_T6_TRGO;
	DACtypeDef.DAC_WaveGeneration=DAC_WaveGeneration_None;
	DAC_Init(DAC_Channel_1,&DACtypeDef);
		DAC_Cmd(DAC_Channel_1,ENABLE);
	DAC_DMACmd(DAC_Channel_1,ENABLE);
}




















































