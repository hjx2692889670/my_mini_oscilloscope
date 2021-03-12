#ifndef _DATAMESSG_H
#define _DATAMESSG_H
#include "stm32_dsp.h"
#include "ADC_DMA_TMR.h"

/*
  DAC定时器数据   TIM6
	rcc：499，pre: 71  Tout=500*72/72 =500us, 默认 1/0.5 KHZ , 1/0.5/len256 points ==xxx---- 15.625Hz  7.8125 Hz   
	每个点之间的间隔 500/256== 1.95 us 
	SUM: 2000/len Hz
	
	ADC 转换时间（越长--->精确）		1/12 *(12.5+13.5)=2.17us 
	定时器数据   TIM3
	rcc:20, pec:1800(ret) Fout =72/20/ret 初始情况 ： Fout=2KHz  500us
	502.17us      1995 Hz    每隔502.17us  发生ADC转换
	
	1995/256==7.79hz
	1/(20*ret/72+2.17)/256
	502.17/256 == 1.96	每点时间间隔1.96us
//SUM:采样频率：256/(2.17+20*ret/72)  *10^6Hz
*/

typedef enum _statue  
{
	START_TASK=0,  
	ADC_TASK,
	DAC_TASK,
	DATA_TASK,
	RUN_TASK, //预选状态
} CUR_TASK;


typedef struct
{
	u8 curTask;
	u8 up;// 				/KEY1
	u8 down;//      //KEY0
	u8 enter;//       //WKUP 一键回到home
	void (*current_operation)();
} key_table;

void doDataPre(void);
#endif

