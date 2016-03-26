#ifndef __LCD_H__
#define __LCD_H__

#define White           0xFFFF
#define Black           0x0000
#define Grey            0xF7DE
#define Blue            0x001F
#define Blue2           0x051F
#define Red             0xF800
#define Magenta         0xF81F
#define Green           0x07E0
#define Cyan            0x7FFF
#define Yellow          0xFFE0
#define Transparent     0xFFFFFF

#define  DISP_WIDTH  240 - 1
#define  DISP_HEIGHT  320 - 1  

typedef enum
{ 
  LCD_LANDSCAPE,
  LCD_PORTRAIT,
}LCD_orient_TypeDef;

typedef struct
{ 
  u8* font;
  u8 x_size;
  u8 y_size;
  u8 offset;
  u8 numchars;
}LCD_Font_TypeDef;

void LCD_Setup(void);
void LCD_Clear(void);

void LCD_SetPixel(u16 Xpos, u16 Ypos);
u16 LCD_GetPixel(u16 Xpos, u16 Ypos);

void LCD_setRGBColor(u8 r, u8 g, u8 b);
void LCD_setColor(u16 color);
u16 LCD_getColor(void);
void LCD_setRGBBackColor(u8 r, u8 g, u8 b);
void LCD_setBackColor(u32 color);
u16 LCD_getBackColor(void);

void LCD_WindowModeDisable(void);
void LCD_DrawLine(u8 Xpos, u16 Ypos, u16 Length, u8 Direction);

void LCD_setFont(u8* font);
void LCD_getFont(LCD_Font_TypeDef* font);
void LCD_printXY(uint8_t *st, uint16_t x, uint16_t y);
void LCD_PrintCharXY(u8 c, u16 x, u16 y);
//void LCD_ConsPrint(u8 *st);
void LCD_BackLight(u8 percent);
/*void LCD_Test(void);*/
void LCD_PrintChar(u8 Chr);
void LCD_ChangeOrientation(LCD_orient_TypeDef Orient);
void LCD_Print(u8 *str);
void LCD_Rectangle(u16 x1, u16 y1, u16 x2, u16 y2);

#endif