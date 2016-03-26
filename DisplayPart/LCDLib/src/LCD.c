#include "fonts.h"
#include "platform_config.h"
#include "LCD.h"
#include "ssd1289.h"

void LCD_SetDisplayWindow(u16 Xpos, u16 Ypos, u16 Width, u16 Height);
void LCD_PrintCharXY(u8 c, u16 x, u16 y);

//static u16 Channel2Pulse = 1000;

//static gl_sFONT *LCD_Currentfonts;

u16 FrontColor = Black;
u16 BackColor = White;
bool _transparent;
LCD_Font_TypeDef  cfont;
u8 CurLn, CurCl, MaxCl, MaxLn;

LCD_orient_TypeDef LCD_orient;
//LCD_orient_TypeDef LCD_orient = LCD_LANDSCAPE;

void LCD_ChangeOrientation(LCD_orient_TypeDef Orient)
{
  LCD_orient = Orient;
  CurLn = 0;
  CurCl = 0;
  
  if (LCD_orient == LCD_LANDSCAPE)
  {
    MaxCl = (DISP_HEIGHT + 1) / cfont.x_size;
    MaxLn = (DISP_WIDTH + 1) / cfont.y_size;
  }
  else
  {
    MaxCl = (DISP_WIDTH + 1) / cfont.x_size;
    MaxLn = (DISP_HEIGHT + 1) / cfont.y_size;
  }
}

//--------------------
void LCD_Setup(void)
{
  Drv_CtrlLinesConfig();
  Drv_FSMCConfig();
  Drv_Init();
}

//--------------------
void LCD_SetDisplayWindow(u16 x1, u16 y1, u16 x2, u16 y2)
{
u16 tmp;

  if (LCD_orient == LCD_LANDSCAPE)
  {
    tmp = x1;
    x1 = y1;
    y1 = tmp;
    tmp = x2;
    x2 = y2;
    y2 = tmp;
    y1 = DISP_HEIGHT - y1;
    y2 = DISP_HEIGHT - y2;
    tmp = y1;
    y1 = y2;
    y2 = tmp;
  }
  if ((x1 > DISP_WIDTH) || (x2 > DISP_WIDTH) || (y1 > DISP_HEIGHT) || (y2 > DISP_HEIGHT))
    return;
  
  Drv_WriteReg(0x44, (x2 << 8) | x1);
  Drv_WriteReg(0x45, y1);
  Drv_WriteReg(0x46, y2);
  
  Drv_WriteReg(0x4e, x1);
  if (LCD_orient == LCD_LANDSCAPE)
    Drv_WriteReg(0x4f, y2);
  else
    Drv_WriteReg(0x4f, y1);

  Drv_WriteRAM_Prepare(); 
}

void LCD_WindowModeDisable(void)
{
  if (LCD_orient == LCD_PORTRAIT)
    LCD_SetDisplayWindow(0, 0, DISP_HEIGHT, DISP_WIDTH);
  else
    LCD_SetDisplayWindow(0, 0, DISP_WIDTH, DISP_HEIGHT);
}

void LCD_Rectangle(u16 x1, u16 y1, u16 x2, u16 y2)
{
u16 i;
  LCD_SetDisplayWindow(x1, y1, x2, y2);
  for (i = 0; i < ((x2 - x1 + 1) * (y2 - y1 + 1)); i++)
    Drv_WriteRAM(FrontColor);
}

void LCD_SetPixel(u16 x, u16 y)
{
  LCD_SetDisplayWindow(x, y, x, y);
  Drv_WriteRAM(FrontColor);
}

u16 LCD_GetPixel(u16 x, u16 y)
{
  LCD_SetDisplayWindow(x, y, x, y);
  return Drv_ReadRAM();
}

//--------------------
void LCD_Clear(void)
{
u32 i;
  if (LCD_orient == LCD_LANDSCAPE)
    LCD_SetDisplayWindow(0, 0, DISP_HEIGHT, DISP_WIDTH);
  else
    LCD_SetDisplayWindow(0, 0, DISP_WIDTH, DISP_HEIGHT);
  
  for(i = 0; i < (DISP_WIDTH + 1) * (DISP_HEIGHT + 1); i++)
    Drv_WriteRAM(BackColor);
}

//--------------------
void LCD_setRGBColor(u8 r, u8 g, u8 b)
{
  FrontColor = ((r&248)<<8 | (g&252)<<3 | (b&248)>>3);
}

//--------------------
void LCD_setColor(u16 color)
{
  FrontColor = color;
}

//--------------------
u16 LCD_getColor(void)
{
  return FrontColor;
}

//--------------------
void LCD_setRGBBackColor(u8 r, u8 g, u8 b)
{
  BackColor = ((r&248)<<8 | (g&252)<<3 | (b&248)>>3);
  _transparent = FALSE;
}

//--------------------
void LCD_setBackColor(u32 color)
{
  if (color == Transparent)
    _transparent = TRUE;
  else
  {
    BackColor = (u16)color;
    _transparent = FALSE;
  }
}

//--------------------
u16 LCD_getBackColor(void)
{
  return BackColor;
}

void LCD_DrawLine(u8 Xpos, u16 Ypos, u16 Length, u8 Direction)
{
  u16 newentrymod;

  if (LCD_orient == LCD_PORTRAIT)
    newentrymod = 0x6030;   // we want a 'vertical line'
  else 
    newentrymod = 0x6018;   // we want a 'horizontal line'
   
  Drv_WriteReg(R11, newentrymod);
  LCD_SetDisplayWindow(Xpos, Ypos, Xpos, Ypos);
  Drv_WriteRAM_Prepare();
  
  while (Length--) {
    Drv_WriteRAM(FrontColor); 
  }
  Drv_WriteReg(R11, 0x6018);
}


//-------------------------
void LCD_printXY(u8 *st, u16 x, u16 y)
{
u8 stl = 0, i;
u8 *tempst;

  tempst = st;
  while (*tempst++ != 0)
    stl++;

  for (i = 0; i < stl; i++)
    LCD_PrintCharXY(*st++, x + (i * (cfont.x_size)), y);
}

//-------------------------
/*void LCD_ConsPrint(u8 *st)
{
static u8 Ln = 0, Cl = 1;
u8 MaxLn, MaxCl;
u16 x, y;
u16 DispX, DispY, col;

  if (LCD_orient == LCD_LANDSCAPE)
  {
    DispX = DISP_HEIGHT + 1;
    DispY = DISP_WIDTH + 1;
  }
  else
  {
    DispX = DISP_WIDTH + 1;
    DispY = DISP_HEIGHT + 1;
  }
  
  MaxCl = DispX / cfont.x_size;
  MaxLn = DispY / cfont.y_size;
  
  if (Ln == MaxLn)
  {
    Ln--;
    for (y = 0; y < DispY - cfont.y_size; y++)
      for (x = 0; x < DispX; x++)
      {
        if (LCD_orient == LCD_LANDSCAPE)
        {
          Drv_SetCursor(y + cfont.y_size, x);
          col = Drv_ReadRAM();
          Drv_SetCursor(y, x);
          Drv_WriteRAM(col);
        }
        else
        {
          Drv_SetCursor(x, y + cfont.y_size);
          col = Drv_ReadRAM();
          Drv_SetCursor(x, y);
          Drv_WriteRAM(col);
        }
      }
  }
  for (x = 0; x < MaxCl; x++)
    LCD_PrintCharXY(' ', x * cfont.x_size, (MaxLn - 1) * cfont.y_size);
  
  LCD_PrintCharXY('>', 0 * cfont.x_size, Ln * cfont.y_size);
  do
  {
    if (Cl == MaxCl)
    {
      Cl = 0;
      Ln++;
    }
    LCD_PrintCharXY(*st++, Cl++ * cfont.x_size, Ln * cfont.y_size);
  }
  while (*st != 0);
  Cl = 1;  
  Ln++;
}*/

void LCD_Print(u8 *str)
{
  while(*str)
  {
    LCD_PrintChar(*str);
    str++;
  }
}

void LCD_PrintChar(u8 Chr)
{
u8 i;
  switch (Chr)
  {
    case '\n':
      CurCl = 0;
      break;
    case '\r':
      if (++CurLn == MaxLn)
        CurLn = 0;
      for(i = 0; i < MaxCl; i++)
        LCD_PrintCharXY(' ', i * cfont.x_size, CurLn * cfont.y_size);
      break;
    default:
      LCD_PrintCharXY(Chr, CurCl * cfont.x_size, CurLn * cfont.y_size);
      CurCl++;
      break;
  }
  
  if (CurCl == MaxCl)
  {
    CurCl = 0;
    if (++CurLn == MaxLn)
      CurLn = 0;
    for(i = 0; i < MaxCl; i++)
      LCD_PrintCharXY(' ', i * cfont.x_size, CurLn * cfont.y_size);
  }  
}

//-----------------------------
void LCD_setFont(u8* font)
{
  cfont.font = font;
  cfont.x_size = font[0];
  cfont.y_size = font[1];
  cfont.offset = font[2];
  cfont.numchars = font[3];
}

//-----------------------------
void LCD_getFont(LCD_Font_TypeDef* font)
{
  font->font = cfont.font;
  font->x_size = cfont.x_size;
  font->y_size = cfont.y_size;
  font->offset = cfont.offset;
  font->numchars = cfont.numchars;
}

//-----------------------------
void LCD_PrintCharXY(u8 c, u16 x, u16 y)
{
u8 i, ch;
u16 j;
u16 temp; 

  if (LCD_orient == LCD_LANDSCAPE)
    Drv_WriteReg(R11, 0x6018);
  else
    Drv_WriteReg(R11, 0x6030);
  
  LCD_SetDisplayWindow(x, y, x + cfont.x_size - 1, y + cfont.y_size - 1);
  
  temp = ((c - cfont.offset) * ((cfont.x_size >> 3) * cfont.y_size)) + 4;
  for(j = 0; j  < ((cfont.x_size >> 3) * cfont.y_size); j++)
  {
    ch = cfont.font[temp];
    for(i = 0; i < 8; i++)
      if((ch&(1 << (7 - i)))!= 0)   
        Drv_PrintPixel(FrontColor);
      else
        if (!_transparent)
          Drv_PrintPixel(BackColor);
        else
          Drv_DummyReadRAM();
    temp++;
  }
  LCD_WindowModeDisable();
}




//++++++++++++++++++++++++++
void LCD_BackLight(u8 percent)
{
  Drv_BackLight(percent);
}

/*void LCD_Test(void)
{
static u8 x = 0x0010, y;
  Drv_WriteReg(0x41, x);
  x += 0x0010;
}*/
