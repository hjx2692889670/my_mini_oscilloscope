#include "ADC_DMA_TMR.h"

u16 dataADC[NTP];
void doAction(u16 arr,u16 psc)
{
		TIM3_Int_Init(arr,psc);   ////(acc+1)*(pre+1)/72mhz   ==  1ms  每隔 tim3发出更新事件，触发ADC转换1/（72/6）*(12.5+1.5)  =1.17us
		Adc_Init();                  
		MYDMA1_Config(DMA1_Channel1,(u32)&ADC1->DR,(u32)&dataADC,NTP);
}
