/*  
		很简易示波器   
		by SDS   2021.2.20日 完成。
    环境 ：MDK  5；
		需要要RTE
		下位机：STM32F103ZET6
    感谢正点以及其他的巨人
************************************
日志：
1:2021.2.27:修改程度较大：修改UI界面，变得简洁，添加了界面选定，程序可拓展
2:在二级菜单里面按倒意料之外的按键 会跳到自己的函数中：{4,		4,	4,	7,showADCWaveUI},
*/
#include "sys.h"
#include "OLED_I2C.h"
#include "key.h"
#include "led.h"
#include "delay.h"
#include "OLED_I2C.h"
#include "dac.h"
#include "adc.h"
#include "dma.h"
#include "timer.h"
#include "DMA_DAC_TMR.h"
#include "ADC_DMA_TMR.h"
#include "usart.h"
#include "OLED_I2C.h"

#include <stdio.h>
#include "GPIO_STM32F10x.h"
#include "dataMessg.h"

#define UART_ENABLE  0  //开启串口
//BMP
//48*48
unsigned char BMP[288]=
{
	 //E:\downloads\1.bmp(0)
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xC0,0xC0,0x60,0x30,0x30,0x18,0x28,0x08,0x14,0x04,0x04,0x00,0x00,0x00,0x00,0x04,0x00,0x00,0x00,0x00,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0xC0,0xF0,0xF8,0x3E,0x0F,0x17,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x04,0x00,0x00,0x00,0x00,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0xF8,0x1F,0x7F,0x07,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x04,0x00,0x00,0x00,0x00,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x60,0x00,0x00,
		0x1F,0xC0,0x00,0x60,0x00,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x20,0x00,0x00,0x00,0x00,0x06,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x06,0x00,0x00,
		0x00,0x00,0x02,0x08,0x01,0x44,0x90,0x28,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x02,0x01,0x04,0x08,0x04,0x18,0x14,0x10,0x20,0x20,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"E:\downloads\6030a0e75d5a2.bmp",0*/
};

extern u16 dataDAC[256];
extern u16 dataADC[NTP];
extern waveType type;
extern u16 len;
extern bool flag;
extern CUR_TASK OS_TASK;//判定程序状态
extern void (*current_operation_index)();

u8 fun_index=0;

u16 ret=1800;
u16 ret2=360;
//跟菜单无关的api
void systemInit(void);
void doShowWave(const u16 *wave);//OLED显示波形

//跟菜单有关的API，
//做处理      //死循环执行
void doADCWave(void);
void doDACWave(void);
void doDataPre(void);
//跟菜单有关的API，
//做UI界面   //只执行一次
void startUI(void);
void showMenu1(void);
void showMenu2(void);
void showMenu3(void);//可根据规律添加

void showADCWaveUI(void);

void showDACWaveUI(void);
void showDACWaveUI2(void);
void showDACWaveUI3(void);
void showDACWaveUI4(void);
void showDACWaveUI5(void);

void showDataUI(void);

key_table  table[]=        //多级菜单
{                        //      1     2      3     1
	{0,		0,	0,	1,startUI}, //过程 ADC-->DATA-->DAC-->ADC
	{1,		3,	2,	4,showMenu1},   //1: ADC
	{2,		1,	3,	5,showMenu2},   //2: DATA
	{3,		2,	1,	6,showMenu3},   //3：DAC
	
	{4,		4,	4,	7,showADCWaveUI},
	{5,		5,	5,	8,showDataUI},
	{6,		13,	10,	9,showDACWaveUI},
	
	{7,		7,	7,	1,doADCWave},
	{8,		8,	8,	2,doDataPre},
	{9,		9,	9,	3,doDACWave},
	
	{10,6,11,9,showDACWaveUI2},
	{11,10,12,9,showDACWaveUI3},
	{12,11,13,9,showDACWaveUI4},
	{13,12,6,9,showDACWaveUI5},
};

int main(void)
 {
	 u8 key;
	 systemInit();   
		 while(1)
		 {
			 key=KEY_Scan(0);
			 if(key&&(OS_TASK==RUN_TASK||OS_TASK==ADC_TASK||OS_TASK==DAC_TASK||OS_TASK==DATA_TASK))
			 {
					LED1=!LED1;
					switch(key)
					 {
						case KEY1_PRES: fun_index=table[fun_index].up;break;
						case KEY0_PRES: fun_index=table[fun_index].down;break;
						case WKUP_PRES:	fun_index=table[fun_index].enter;break;				 
					 }					 
							current_operation_index=table[fun_index].current_operation;
			 }
				current_operation_index();															
		 }
}  
//start  UI
 /*

   0
*/
void startUI()
{	
		u8 key;
			OLED_ShowStr(88,36,(u8 *)"---SDS",12,1);
			OLED_Refresh_Gram();
			OLED_DrawBMP(40,0,88,5,BMP);//(5+1)*8 	
	  while(OS_TASK!=RUN_TASK)
		{
			key=KEY_Scan(0);
			if(key==WKUP_PRES)  
			{
				LED1=0;
			  OS_TASK=RUN_TASK;
				fun_index=table[fun_index].enter;
				current_operation_index=table[fun_index].current_operation;
				break;
			}
		}
}

 /*

   1
*/
void showMenu1()
{
	OLED_CLS();
	OLED_ShowStr(30,24, (u8 *)"ADC",24,1);
	OLED_ShowStr(30,0,	(u8 *)"UP  :DAC",12,1);
	OLED_ShowStr(30,48, (u8 *)"DOWN:DAT",12,1);
	OLED_Refresh_Gram();
}
 /*

   2
*/
void showMenu2()
{
	OLED_CLS();
	OLED_ShowStr(30,24, (u8 *)"DAT",24,1);
	OLED_ShowStr(30,0,	(u8 *)"UP  :ADC",12,1);
	OLED_ShowStr(30,48, (u8 *)"DOWN:DAC",12,1);
	OLED_Refresh_Gram();
}
 /*

  3
*/
void showMenu3()
{
	OLED_CLS();
	OLED_ShowStr(30,24, (u8 *)"DAC",24,1);
	OLED_ShowStr(30,0,	(u8 *)"UP  :DAT",12,1);
	OLED_ShowStr(30,48, (u8 *)"DOWN:ADC",12,1);
	OLED_Refresh_Gram();
}
 /*

  4
*/
void showADCWaveUI()
{
		double f;
		u16 I;
   	OLED_CLS();
		OLED_ShowStr(20,0,(u8 *)"ADC fre:Hz:",16,1);
		f=1000000.0/((20*ret/72+2.17));   //1000/(20*ret/72+2.17)/256KHz
		OLED_ShowNum(20,16,f,5,12); //保留小数点后三位
		OLED_ShowChar(50,16,'.',12,1);//
			I=f;
			f-=I;
		OLED_ShowNum(56,16,f*1000,3,12);//小数点后三位
		OLED_Refresh_Gram();
				// delay_ms(500);
}
 /*
  5
*/ 
void showDataUI()
{
   	double f;
		u16 I;
   	OLED_CLS();
	OLED_ShowStr(20,0,(u8 *)"ADC fre:Hz:",16,1);
		f=1000000.0/((20*ret/72+2.17)*256);   //  频率分辨率：1/(2.17+20*ret/72)  *10^6Hz
		OLED_ShowNum(20,16,f,5,12); //保留小数点后三位
		OLED_ShowChar(50,16,'.',12,1);//
			I=f;
			f-=I;
		OLED_ShowNum(56,16,f*1000,3,12);//小数点后三位
		OLED_Refresh_Gram();
				 //delay_ms(500);
}
 /*
  6
*/
void showDACWaveUI()
{
		u16 I;
		double f;
	 	OLED_CLS();
		OLED_ShowStr(20,0,(u8 *)"WAVE_SINE",16,1);
		OLED_ShowStr(20,16,(u8 *)"DAC fre:Hz:",16,1);
		f=2000.0/len; // 
		OLED_ShowNum(20,32,f,5,12);
		OLED_ShowChar(50,32,'.',12,1);
    I=f;
		f-=I;
		OLED_ShowNum(56,32,f*1000,3,12);
		OLED_Refresh_Gram();
		type=WAVE_SINE;
	resetWave(100-1,ret2-1);
				 //delay_ms(500);
}
void showDACWaveUI2()
{
	 OLED_ShowStr(20,0,(u8 *)"WAVE_RAMP",16,1);
	 OLED_myRefresh_Gram();//front two byte
	type=WAVE_RAMP;
					resetWave(100-1,ret2-1);
}
void showDACWaveUI3()
{
	 OLED_ShowStr(20,0,(u8 *)"WAVE_SQUARE",16,1);
	 OLED_myRefresh_Gram();//front two byte
		type=WAVE_SQUARE;
					resetWave(100-1,ret2-1);
}
void showDACWaveUI4()
{
	 OLED_ShowStr(20,0,(u8 *)"WAVE_TRIANGLE",16,1);
	 OLED_myRefresh_Gram();//front two byte
	type=WAVE_TRIANGLE;
	resetWave(100-1,ret2-1);
}
void showDACWaveUI5()
{
	 OLED_ShowStr(20,0,(u8 *)"WAVE_MUX",16,1);
	 OLED_myRefresh_Gram();//front two byte
	type=WAVE_MUX;
	resetWave(100-1,ret2-1);
}
/*
  7
*/
void doADCWave()
{
	u8 key;
	OS_TASK=ADC_TASK;
	TIM_Cmd(TIM3,ENABLE);
	while(OS_TASK==ADC_TASK)
	{
			key = KEY_Scan(0);
			if(key)  
			{
				LED1=~LED1;
				switch(key)
				{
					case KEY0_PRES:ret+=100;
													ret= ret> 65535 ? 65535:ret;
												 TIM_PrescalerConfig(TIM3,ret-1,TIM_PSCReloadMode_Update);break;    //向下翻
					case KEY1_PRES:	ret-=100;
													 ret= ret< 1 ? 1:ret;
													TIM_PrescalerConfig(TIM3,ret-1,TIM_PSCReloadMode_Update);break;   	//向上翻
					case WKUP_PRES:OS_TASK=RUN_TASK;//退出循环	
										fun_index=table[fun_index].enter;									
										current_operation_index=table[fun_index].current_operation;	
										break;             
				}		
			}
				doShowWave(dataADC);
		}
	  TIM_Cmd(TIM3,DISABLE);
}
 /*

  9
*/
void  doDACWave()
{
	u8 key;
	OS_TASK=DAC_TASK;
	while(OS_TASK==DAC_TASK)
	{
	   key = KEY_Scan(0);
		if(key)  
		{
			LED1=~LED1;
			switch(key)
			{
					case KEY0_PRES: len+=32;
													len=len>65535?65535:len;
														break;
					case KEY1_PRES:	len-=32;
													len=len<128?128:len;
														break;				
							case WKUP_PRES:OS_TASK=RUN_TASK; //退出
									 fun_index=table[fun_index].enter;
							current_operation_index=table[fun_index].current_operation; //根据状态进行函数赋值				
							break;
			}						
				resetWave(100-1,360-1);
		}
			doShowWave(dataDAC);
	}	
}
 /*

  各种外设初始化均放到此处
*/
void systemInit()
{
  delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
 	LED_Init();			     //LED端口初始化
	OLED_Init();			//初始化OLED 
	KEY_Init();	 
	 LED0=1;
	 LED1=1;	
#if UART_ENABLE
	 uart_init(115200); 
#endif	
	 resetWave(100-1,ret2-1);
	TIM_Cmd(TIM6,ENABLE);
	
	 doAction(20-1,ret-1);
		OS_TASK=START_TASK;
	 current_operation_index=table[0].current_operation; //根据状态进行函数赋值
}

 /*
  打印wave BUFF中的数据
*/
void doShowWave(const u16 *wave)
{
		u16 x=0; u16 y=0;	
		u16 cur;
	//while(x+y<=NTP)  //连续效果
	//{
				OLED_CLS();
	   		for(x=0;x<128;x++)  
			{				
					cur=wave[x+y]*63/4095;
					OLED_DrawPoint(x,cur,1);
			}
				OLED_Refresh_Gram(); 			 
				//y+=x;	
//	}

}
