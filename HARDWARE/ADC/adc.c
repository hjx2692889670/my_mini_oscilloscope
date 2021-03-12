/**********************************************************
					使用ADC1 CH6 做ADC入口检测
***********************************************************/ 
 #include "adc.h"
 
void Adc_Init(void)
{
		ADC_InitTypeDef ADCtypeDef;
		GPIO_PortClock(GPIOA,ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	  		ADC_DeInit(ADC1);
		RCC_ADCCLKConfig(RCC_PCLK2_Div6);
		ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 1, ADC_SampleTime_13Cycles5);//    1/12 *(12.5+13.5)  =2.17us

		GPIO_PinConfigure(GPIOA,6,GPIO_IN_ANALOG,GPIO_MODE_INPUT);//72/6=12MHz	

	ADCtypeDef.ADC_ContinuousConvMode=DISABLE;
	ADCtypeDef.ADC_DataAlign=ADC_DataAlign_Right;
	ADCtypeDef.ADC_ExternalTrigConv=ADC_ExternalTrigConv_T3_TRGO;
	ADCtypeDef.ADC_Mode=ADC_Mode_Independent;
	ADCtypeDef.ADC_NbrOfChannel=1;
	ADCtypeDef.ADC_ScanConvMode=DISABLE;
		ADC_Init(ADC1,&ADCtypeDef);
	
	ADC_Cmd(ADC1,ENABLE);
	ADC_DMACmd(ADC1,ENABLE);
	
	ADC_ResetCalibration(ADC1);
	while(ADC_GetResetCalibrationStatus(ADC1));
	ADC_StartCalibration(ADC1);
	while(ADC_GetCalibrationStatus(ADC1));
	ADC_ExternalTrigConvCmd(ADC1,ENABLE);
}



























