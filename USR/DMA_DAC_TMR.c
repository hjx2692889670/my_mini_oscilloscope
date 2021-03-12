#include "DMA_DAC_TMR.h"
#include "key.h"

u16 dataDAC[256];

waveType type=WAVE_SINE;
u16 len=256,data_vpp=3300;

void wave_gen(waveType type, u16 len, u16 vpp)
{
    u16 base = 2048; // 
    double amp; 
    u16 i; // 
    if (vpp > 3300)
        vpp = 3300; // 
    amp = vpp / 3300.0 * 2047.0; //0---2047 
																		//base +amp=  2048+2048 ---->4096
    if (type == WAVE_SINE) //  Èý½Ç²¨
    {
        for (i = 0; i < len; i++)
        {
            dataDAC[i] = (u16)(base + sin((double)i / len * 2*pi) * amp);
        }
    }
    else if (type == WAVE_RAMP) //Ð±²¨ 
    {
        for (i = 0; i < len; i++)
        {
            dataDAC[i] = (u16)(base + ((double)i / len - 0.5) * 2 * amp);
        }
    }
    else if (type == WAVE_SQUARE) // ·½²¨
    {
        for (i = 0; i < len / 2; i++)
        {
            dataDAC[i] = base + amp;
        }
        for (i = len / 2; i < len; i++)
        {
            dataDAC[i] = base - amp;
        }
    }
    else if (type == WAVE_TRIANGLE) // ½Ç²¨
    {
        for (i = 0; i < len / 2; i++)
        {
            dataDAC[i] = (u16)(base + ((double)i / len - 0.25) * 4 * amp);
        }
        for (i = len / 2; i < len; i++)
        {
            dataDAC[i] = (u16)(base + ((double)(len - i) / len - 0.25) * 4 * amp);
        }
    }
		else if(type==WAVE_MUX)   //»ìºÏ
		{
				u16 i;
				double fx;
				for(i=0; i<len; i++)
				{
						fx = 0.5*sin((double)(pi*2*i)/len)+0.5*cos((double)2*pi*i/len+pi*0.3);
						dataDAC[i] = (u16)(base+amp*fx);
				}

		}
}

void resetWave(u16 arr,u16 psc)
{
    wave_gen(type,len,data_vpp);	
	DAC_Cmd(DAC_Channel_1,DISABLE);
	DAC_DMACmd(DAC_Channel_1,DISABLE);
	DMA_Cmd(DMA2_Channel3,DISABLE);	
			TIM_Cmd(TIM6,DISABLE);
	
		Dac1_Init();
	 	TIM6_Int_Init(arr,psc);	// 
 //(acc+1)*(pre+1)/72mhz  
	MYDMA2_Config(DMA2_Channel3,(u32)&DAC->DHR12R1,(u32)dataDAC,len);
	TIM_Cmd(TIM6,ENABLE);
}

