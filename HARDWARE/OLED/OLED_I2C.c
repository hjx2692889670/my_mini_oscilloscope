#include "OLED_I2C.h"
#include "oledfont.h"
#include "delay.h"
/**
 Function List:
  
	void OLED_Init(void);// -- OLED										 	PB6->SCL
                         //                         	PB7->SDA
  uint32_t Write_Byte_to_OLED(u8 Cmd_or_Data,u8 Data)//??I2C?Cmd_or_Data??Data
	void WriteCmd(unsigned char I2C_Command);// -- ???
	void WriteDat(unsigned char I2C_Data);// -- ???
	
	
	void OLED_Refresh_Gram(void);//??
	void OLED_ON(void);// -- ??
	void OLED_OFF(void);//) -- ??
	void OLED_SetPos(unsigned char x, unsigned char y);// -- ???????
	void OLED_Fill(unsigned char fill_Data);// -- ????
	void OLED_CLS(void);// -- ??
	void OLED_DrawPoint(u8 x,u8 y,u8 t);// --??
	void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size,u8 mode);// --??????
	void OLED_ShowStr(u8 x,u8 y,const u8 *p,u8 size,u8 mode);
	void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char N);// -- ????(???????,????oledfont.h?)
	void OLED_DrawBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[]);// -- BMP??
    *
**/ 
 
u8 OLED_GRAM[128][8];
uint8_t Bef[3];//????????????1.??????2.0x01?????3.?????
uint8_t Cur[3];//???????1.??????2.0x01?????3.?????

static __IO uint32_t  I2CTimeout = I2CT_LONG_TIMEOUT;
static uint32_t I2C_TIMEOUT_UserCallback(uint8_t errorCode);
 
/************************************
  * @brief  Write_Byte_to_OLED,HARDWARE I2C 
  * @param  ?
  * @retval ?
  ***********************************/
uint32_t Write_Byte_to_OLED(u8 Cmd_or_Data,u8 Data)
{
  I2C_GenerateSTART(OLED_I2C, ENABLE);																					                
     //????
  I2CTimeout = I2CT_FLAG_TIMEOUT;
  while(!I2C_CheckEvent(OLED_I2C, I2C_EVENT_MASTER_MODE_SELECT))								        
     //EV5
  {
    if((I2CTimeout--) == 0)
		return I2C_TIMEOUT_UserCallback(0);
  }
	
  I2C_Send7bitAddress(OLED_I2C, OLED_ADDRESS, I2C_Direction_Transmitter);			  
  I2CTimeout = I2CT_FLAG_TIMEOUT;
  while(!I2C_CheckEvent(OLED_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))	
  {
    if((I2CTimeout--) == 0)
		return I2C_TIMEOUT_UserCallback(1);
  }    
      
  I2C_SendData(OLED_I2C, Cmd_or_Data);																					     
     //??????????,??0x00,??0x40
  I2CTimeout = I2CT_FLAG_TIMEOUT;
  while(!I2C_CheckEvent(OLED_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED))  					        
     //EV8 and clear it */
  {
    if((I2CTimeout--) == 0)
		return I2C_TIMEOUT_UserCallback(2);
  }
	
  I2C_SendData(OLED_I2C, Data);																									//???????? 
  I2CTimeout = I2CT_FLAG_TIMEOUT;
  while(!I2C_CheckEvent(OLED_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED))						//EV8 and clear it */
  {
    if((I2CTimeout--) == 0)
		return I2C_TIMEOUT_UserCallback(3);
  } 
  
  I2C_GenerateSTOP(OLED_I2C, ENABLE);																						//????
  
  return 1;
}
/************************************
  * @brief  OLED_Init,???OLED
  * @param  ?
  * @retval ?
  ***********************************/
void OLED_Init(void)
{   
	
	GPIO_InitTypeDef GPIO_InitStructure;
	I2C_InitTypeDef I2C_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
	I2C_DeInit(I2C1);
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_OwnAddress1 = OWN_ADDRESS;
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_ClockSpeed = I2C_SPEED;
	I2C_Init(I2C1, &I2C_InitStructure);
	I2C_Cmd(I2C1, ENABLE);
 
 
	
	delay_ms(1000);     // 1s,????????,?????,?????????
	WriteCmd(0xAE);//--display off
	WriteCmd(0x00);//---set low column address
	WriteCmd(0x10);//---set high column address
	WriteCmd(0x40);//--set start line address  
	WriteCmd(0xB0);//--set page address
	WriteCmd(0x81); // contract control
	WriteCmd(0xFF);//--128   
	WriteCmd(0xA1);//set segment remap 
	WriteCmd(0xA6);//--normal / reverse
	WriteCmd(0xA8);//--set multiplex ratio(1 to 64)
	WriteCmd(0x3F);//--1/32 duty
	WriteCmd(0xC8);//Com scan direction
	WriteCmd(0xD3);//-set display offset
	WriteCmd(0x00);//
	
	WriteCmd(0xD5);//set osc division
	WriteCmd(0x80);//
	
	WriteCmd(0xD8);//set area color mode off
	WriteCmd(0x05);//
	
	WriteCmd(0xD9);//Set Pre-Charge Period
	WriteCmd(0xF1);//
	
	WriteCmd(0xDA);//set com pin configuartion
	WriteCmd(0x12);//
	
	WriteCmd(0xDB);//set Vcomh
	WriteCmd(0x30);//
	
	WriteCmd(0x8D);//set charge pump enable
	WriteCmd(0x14);//
	
	WriteCmd(0xAF);//--turn on OLED panel
	
	
}
 
 
 
 
/**********************************************************************
  * @brief  WriteCmd,?OLED????
  * @param  I2C_Command:????
  * @retval ?
  *********************************************************************/
void WriteCmd(unsigned char I2C_Command)//???
{
   Write_Byte_to_OLED(0x00, I2C_Command);
}
 
 
 /**********************************************************************
  * @brief  OLED_WriteDat
  * @param  I2C_Data:??
  * @retval ?
  *********************************************************************/
void WriteDat(unsigned char I2C_Data)//???
{
   Write_Byte_to_OLED(0x40, I2C_Data);
}
 
/*****************************************************Draw********************************************/
//?????OLED	 
void OLED_Refresh_Gram(void)
{
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		WriteCmd(0xb0+i);    
		WriteCmd(0x00);      
		WriteCmd(0x10);        
		for(n=0;n<128;n++)WriteDat(OLED_GRAM[n][i]); 
	}   
}
 /*************************************************************************
  * @brief  OLED_ON,?OLED??????
  * @param  ?
    * @retval ?
  ************************************************************************/
void OLED_ON(void)
{
    WriteCmd(0X8D);  //?????
    WriteCmd(0X14);  //?????
    WriteCmd(0XAF);  //OLED??
}
 /*************************************************************************
  * @brief  OLED_OFF,?OLED?? -- ?????,OLED????10uA
  * @param  ?
    * @retval ?
  ************************************************************************/
void OLED_OFF(void)
{
    WriteCmd(0X8D);  //?????
    WriteCmd(0X10);  //?????
    WriteCmd(0XAE);  //OLED??
}
/*************************************************************************
  * @brief  OLED_SetPos,????
  * @param  x,??x??
    *                   y,??y??
  * @retval ?
  ************************************************************************/
void OLED_SetPos(unsigned char x, unsigned char y) 
{ 
    WriteCmd(0xb0+y);
    WriteCmd(((x&0xf0)>>4)|0x10);
    WriteCmd((x&0x0f)|0x01);
}
 /*************************************************************************
  * @brief  OLED_Fill,??????
  * @param  fill_Data:??????
    * @retval ?
  ************************************************************************/
void OLED_Fill(unsigned char fill_Data)
{
    unsigned char m,n;
    for(m=0;m<8;m++)
    {
        WriteCmd(0xb0+m);    //page0-page1
        WriteCmd(0x00);     //low column start address
        WriteCmd(0x10);     //high column start address
        for(n=0;n<128;n++)
            {
                WriteDat(fill_Data);
				OLED_GRAM[n][m] = fill_Data;
            }
    }
}
 /*************************************************************************
  * @brief  OLED_Fill,??????
  * @param  fill_Data:??????
    * @retval ?
  ************************************************************************/
void OLED_GRAM_FILL(unsigned char fill_Data)//????
{
    unsigned char m,n;
    for(m=0;m<8;m++)
    {
        WriteCmd(0xb0+m);    //page0-page1
        WriteCmd(0x00);     //low column start address
        WriteCmd(0x10);     //high column start address
        for(n=0;n<128;n++)
            {
				OLED_GRAM[n][m] = fill_Data;
            }
    }
}
 /*************************************************************************
  * @brief  OLED_CLS,??
  * @param  ?
    * @retval ?
  ************************************************************************/
void OLED_CLS(void)//??
{
    OLED_Fill(0x00);
}
 /*************************************************************************
  * @brief  OLED_DrawPoint ?? 
  * @param  x:0~127
  *         y:0~63
  *        t:1 ?? 0,??		
  * @retval ?
  ************************************************************************/   
void OLED_DrawPoint(u8 x,u8 y,u8 t)
{
	u8 pos,bx,temp=0;
	if(x>127||y>63)return;
	pos=y/8;
	bx=y%8;
	temp=1<<(bx);
	if(t)OLED_GRAM[x][pos]|=temp;
	else OLED_GRAM[x][pos]&=~temp;	    
}
 /*************************************************************************
  * @brief  OLED_char,??????,?6*8?16*16 24*24???
  * @param  x,y : ?????(x:0~127, y:0~63);
    *       chr :- ??????; 
    *       Size : ????(12/16/24)
    *       mode:0,????;1,????
    * @retval ?
  ************************************************************************/
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size,u8 mode)
{      			    
	u8 temp,t,t1;
	u8 y0=y;
	u8 csize=(size/8+((size%8)?1:0))*(size/2);		
	chr=chr-' '; 
    for(t=0;t<csize;t++)
    {   
		if(size==12)temp=asc2_1206[chr][t]; 	 	
		else if(size==16)temp=asc2_1608[chr][t];	
		else if(size==24)temp=asc2_2412[chr][t];	
		else return;								
    for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)OLED_DrawPoint(x,y,mode);
			else OLED_DrawPoint(x,y,!mode);
			temp<<=1;
			y++;
			if((y-y0)==size)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
   }          
}
 
 /*************************************************************************
  * @brief  OLED_ShowStr,??oledfont.h??ASCII??,?6*8?16*16 24*24???
  * @param  x,y : ?????(x:0~127, y:0~63);
    *       ch[] :- ???????; 
    *       TextSize :????(12/16/24)
    * @retval ?
  ************************************************************************/
void OLED_ShowStr(u8 x,u8 y,const u8 *p,u8 size,u8 mode)
{
    while((*p<='~')&&(*p>=' '))//?????????!
    {       
        if(x>(128-(size/2))){x=0;y+=size;}
        if(y>(64-size)){y=x=0;OLED_CLS();}
        OLED_ShowChar(x,y,*p,size,mode);	 
        x+=size/2;
        p++;
    }  
}
 
void OLED_DrawBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[])
{
    unsigned int j=0;
    unsigned char x,y;

  if(y1%8==0)
        y = y1/8;
  else
        y = y1/8 + 1;
    for(y=y0;y<y1;y++)
    {
        OLED_SetPos(x0,y);
    for(x=x0;x<x1;x++)
        {
            WriteDat(BMP[j++]);
        }
    }
}


static  uint32_t I2C_TIMEOUT_UserCallback(uint8_t errorCode)
{
  /* Block communication and all processes */
 // EEPROM_ERROR("I2C ????!errorCode = %d",errorCode);
  return 0;
}
  //&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
 /************************************************************************
  * @brief  OLED_myCLR(); 
  * @param  
  *        
  * @retval NULL
  ************************************************************************/
void OLED_myCLS()//?????16*64??
{
	 unsigned char m,n;
    for(m=0;m<2;m++)
    {
        WriteCmd(0xb0+m);    //page0-page1
        WriteCmd(0x00);     //low column start address
        WriteCmd(0x10);     //high column start address
      for(n=0;n<128;n++)
        {
           WriteDat(0x00);
					 OLED_GRAM[n][m]=0x00;
        }
    }
}

 /*************************************************************************
  * @brief  OLED_ShowCN,?GRam
  * @param  x,y: ?????(x:0~127, y:0~7); 
    *                   N:???codetab.h????
    * @retval ?
  ************************************************************************/
void OLED_myShowCN(unsigned char x, unsigned char y, unsigned char N)
{
    unsigned char wm=0;
    unsigned int  adder=32*N;
    OLED_SetPos(x , y);
    for(wm = 0;wm < 16;wm++)
    {
        WriteDat(CN16x16[adder]);
        adder += 1;
    }
    OLED_SetPos(x,y + 1);
    for(wm = 0;wm < 16;wm++)
    {
        WriteDat(CN16x16[adder]);
        adder += 1;
    }
}
void OLED_myRefresh_Gram(void)
{
	u8 i,n;		    
	for(i=0;i<2;i++)  
	{  
		WriteCmd(0xb0+i);    
		WriteCmd(0x00);      
		WriteCmd(0x10);        
		for(n=0;n<128;n++)WriteDat(OLED_GRAM[n][i]); 
	}   
}

void OLED_DrawLine(u8 x1,u8 y1,u8 x2,u8 y2,u8 mode)
{
        u8 i,k,k1,k2;    
        if(x1==x2)    
        {
                        for(i=0;i<(y2-y1);i++)
                        {
                                OLED_DrawPoint(x1,y1+i,1);
                        }
  }
        else if(y1==y2)   
        {
                        for(i=0;i<(x2-x1);i++)
                        {
                                OLED_DrawPoint(x1+i,y1,1);
                        }
  }
        else      
        {
                k1=y2-y1;
                k2=x2-x1;
                k=k1*10/k2;
                for(i=0;i<(x2-x1);i++)
                        {
                          OLED_DrawPoint(x1+i,y1+i*k/10,1);
                        }
        }
}

/*

*/
//m^n函数
u32 mypow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}				  
//显示2个数字
//x,y :起点坐标	 
//len :数字的位数
//size:字体大小
//mode:模式	0,填充模式;1,叠加模式
//num:数值(0~4294967295);	 		  
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size)
{         	
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/mypow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(size/2)*t,y,' ',size,1);
				continue;
			}else enshow=1; 
		 	 
		}
	 	OLED_ShowChar(x+(size/2)*t,y,temp+'0',size,1); 
	}
} 
     


//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ 
/*********************************************END OF FILE**********************/
