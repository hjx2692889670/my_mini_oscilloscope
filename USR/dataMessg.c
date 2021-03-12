/******************************************************************


*******************************************************************/
#include "dataMessg.h"
#include "math.h"
#include "stdio.h"
#include "string.h"
#include "key.h"
#include "OLED_I2C.h"
#include "led.h"
extern u16 dataADC[NTP];
extern key_table table[4];
extern u16 ret;
extern u8 fun_index;
bool flag=false;  //用来判断是否发生了数据处理 ，保证只要发一次
CUR_TASK OS_TASK;

void (*current_operation_index)();
signed int  lBufInArray[NTP];    
signed int lBufOutArray[NTP/2]; 
signed int lBufMagArray[NTP/2]; 

 
void GetPowerMag(void)
{              
    float X,Y,Mag;                                                      
    unsigned short i;
    for(i=0; i<NTP/2; i++)                        //FFT  A0=lBufOutArray[0]/NPT
    {                                             //   Ai=lBufOutArray[i]*2/NPT
        X  = (lBufOutArray[i] << 16) >> 16;  //lX  = lBufOutArray[i];
        Y  = (lBufOutArray[i] >> 16);
        Mag = sqrt(X * X + Y * Y);
        if(i == 0)
            lBufMagArray[i] = (Mag);   //
        else
            lBufMagArray[i] = (Mag*2);
    }
}

void adc_sample(void)
{
	u16 i=0;
	memset(lBufMagArray,0,NTP/2);
	memset(lBufOutArray,0,NTP/2);
	memset(lBufInArray,0,NTP);
    for (i = 0; i < NTP; i++)
    {
        lBufInArray[i] = dataADC[i];
    }
    cr4_fft_256_stm32(lBufOutArray,lBufInArray, NTP);
    GetPowerMag();
}
/*
  8
*/
void doDataPre()
{
	u16 i=0,x,t=0;
	u8 key;  
	u16 j=0;
	OS_TASK=DATA_TASK;
	flag=false;//每次adc都会刷新flag  以进行串口发送数据
	while(OS_TASK==DATA_TASK)
	{
			 key=KEY_Scan(0);
				//delay_ms(300);
			 if(key)
			 {
					t=0;
				 	LED1=~LED1;
					switch(key)
					 {
						case KEY1_PRES:
													j=j<10?10:j;	
													j-=10;       //向上翻页 										
													for(i=0;i<5;i++)       //数据  5行 2列
													{
														for(x=0;x<2;x++)
														{
																OLED_ShowNum((x)*63,(i*12),lBufMagArray[j+t],6,12);
																t++;
														}
													}
													break;
						case KEY0_PRES:	j+=10;       //向下 
													j=j>(NTP/2 -10) ? (NTP/2 -10):j;	
													OLED_CLS();
													for(i=0;i<5;i++)       //数据  5行 2列
													{
														for(x=0;x<2;x++)
														{
																OLED_ShowNum((x*63),(i*12),lBufMagArray[t+j],6,12);
															t++;
														}
													}			
													break;
						case WKUP_PRES:OS_TASK=RUN_TASK;//exit loop	
							fun_index=table[fun_index].enter;
							current_operation_index=table[fun_index].current_operation;						
							break;				 
					 }
						OLED_Refresh_Gram();//updata the data in oled					 				
			 }		
		if(!flag)
		{
			 adc_sample();
#if UART_ENABLE
			for(i=0;i<NTP/2;i++)       //串口发送数据
			{
				printf(" %d \r\n",lBufMagArray[i]);
			}
				printf("data sent over \r\n");
#endif
				flag=true;
				OLED_CLS();
				OLED_ShowStr(0,0,(u8 *)"OK,now,tim3 rcc:",12,1);
				OLED_ShowNum(0,12,ret,5,12);
				OLED_Refresh_Gram();
				delay_ms(1000);
				OLED_CLS();
				OLED_ShowStr(0,0,(u8 *)"DATA:",12,1);
				OLED_Refresh_Gram();//updata the data in oled					 				
		}		
	}
}
