#ifndef __OLED_H
#define	__OLED_H
 
#include "stm32f10x.h"
#include "sys.h"
 
#define I2C_SPEED           400000	
#define OWN_ADDRESS          0X77
#define OLED_ADDRESS         0X78 
#define OLED_I2C             I2C1

#define I2CT_FLAG_TIMEOUT         		((uint32_t)300)
#define I2CT_LONG_TIMEOUT         		((uint32_t)(10 * I2CT_FLAG_TIMEOUT))				

#define EEPROM_ERROR(fmt,arg...)       printf("<<-OLED-ERROR->> "fmt"\n",##arg)
 
 
void OLED_Init(void);
void WriteCmd(unsigned char I2C_Command);
void WriteDat(unsigned char I2C_Data);
 
void Oled_WriteByte(u8 reg_add,u8 reg_dat);
void OLED_GRAM_FILL(unsigned char fill_Data);
void OLED_Refresh_Gram(void);
void OLED_ON(void);
void OLED_OFF(void);
void OLED_SetPos(unsigned char x, unsigned char y);
void OLED_Fill(unsigned char fill_Data);
void OLED_CLS(void);
void OLED_DrawPoint(u8 x,u8 y,u8 t);
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size,u8 mode);
void OLED_ShowStr(u8 x,u8 y,const u8 *p,u8 size,u8 mode);
void OLED_DrawBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char* BMP);



void OLED_DrawLine(u8 x0,u8 y0,u8 x1,u8 y1,u8 mode);
void OLED_myShowCN(unsigned char x, unsigned char y, unsigned char N);
void OLED_myCLS(void);//?????16*64??
void OLED_myRefresh_Gram(void);
void OLED_myDrawPoints(u16 *data, u16 _usSize);
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size);
#endif
