#ifndef __SSD1289_H__
#define __SSD1289_H__

#include "fonts.h"

#define R11 0x11
#define R34 0x22

//#define LCD_PIXEL_WIDTH          0x0140
//#define LCD_PIXEL_HEIGHT         0x00F0

void Drv_CtrlLinesConfig(void);
void Drv_FSMCConfig(void);
void Drv_Init(void);

void Drv_WriteReg(u8 LCD_Reg,u16 LCD_RegValue);
u16 Drv_ReadReg(u8 LCD_Reg);
void Drv_WriteRAM_Prepare(void);
void Drv_WriteRAM(u16 RGB_Code);					 
u16 Drv_ReadRAM(void);
void Drv_DummyReadRAM(void);
void Drv_SetCursor(u16 x, u16 y);
void Drv_PrintPixel(u16 color);
/*void Drv_PowerOn(void);
void Drv_DisplayOn(void);
void Drv_DisplayOff(void);*/
void Drv_BackLight(u8 percent);

#endif