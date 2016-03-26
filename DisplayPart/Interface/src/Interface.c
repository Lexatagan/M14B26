/* Includes ------------------------------------------------------------------*/
#include "platform_config.h"
#include "Interface.h"
#include "LCD.h"
#include "DBG.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/ 
/* Extern variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

void If_PrintCylCell(uint8_t N, uint16_t temp)
{
uint16_t color;
uint8_t a, b;
  if (N > 8)
    return;
  a = N % 3;
  b = N / 3;
  if (temp < GREEN_THRESHOLD)
    color = Yellow;
  if ((temp >= GREEN_THRESHOLD) && (temp < RED_THRESHOLD))
    color = Green;
  if (temp >= RED_THRESHOLD)
    color = Red;
  
  LCD_setColor(color);
  LCD_Rectangle(5 + a * 80, 5 + b * 80, 75 + a * 80, 29 + b * 80);
  LCD_Rectangle(5 + a * 80, 46 + b * 80, 75 + a * 80, 75 + b * 80);
  LCD_Rectangle(5 + a * 80, 30 + b * 80, 14 + a * 80, 45 + b * 80);
  LCD_Rectangle(63 + a * 80, 30 + b * 80, 75 + a * 80, 45 + b * 80);

  LCD_setColor(0x0000);
  LCD_setBackColor(color);
  DBG_PrintDEC_XXXgrad(temp / 10, a * 80 + 15, b * 80 + 30);
}

void If_PrintOilTemp(uint16_t temp)
{
uint16_t color;
uint8_t a, b;

  a = 3;
  b = 0;

  if (temp < GREEN_THRESHOLD)
    color = Yellow;
  if ((temp >= GREEN_THRESHOLD) && (temp < RED_THRESHOLD))
    color = Green;
  if (temp >= RED_THRESHOLD)
    color = Red;
  
  LCD_setColor(color);
  LCD_Rectangle(5 + a * 80, 5 + b * 80, 75 + a * 80, 29 + b * 80);
  LCD_Rectangle(5 + a * 80, 30 + b * 80, 14 + a * 80, 45 + b * 80);
  LCD_Rectangle(63 + a * 80, 30 + b * 80, 75 + a * 80, 45 + b * 80);
  LCD_Rectangle(5 + a * 80, 46 + b * 80, 29 + a * 80, 61 + b * 80);
  LCD_Rectangle(46 + a * 80, 46 + b * 80, 75 + a * 80, 61 + b * 80);
  LCD_Rectangle(5 + a * 80, 62 + b * 80, 75 + a * 80, 75 + b * 80);

  LCD_setColor(0x0000);
  LCD_setBackColor(color);
  DBG_PrintDEC_XXXgrad(temp / 10, a * 80 + 15, b * 80 + 30);
  LCD_printXY("t", a * 80 + 30, b * 80 + 46);
}

void If_PrintOilPressure(uint16_t pressure)
{
uint16_t color;
uint8_t a, b;

  a = 3;
  b = 1;

  if (pressure < GREEN_THRESHOLD)
    color = Yellow;
  if ((pressure >= GREEN_THRESHOLD) && (pressure < RED_THRESHOLD))
    color = Green;
  if (pressure >= RED_THRESHOLD)
    color = Red;
  
  LCD_setColor(color);
  LCD_Rectangle(5 + a * 80, 5 + b * 80, 75 + a * 80, 29 + b * 80);
  LCD_Rectangle(5 + a * 80, 30 + b * 80, 14 + a * 80, 45 + b * 80);
  LCD_Rectangle(63 + a * 80, 30 + b * 80, 75 + a * 80, 45 + b * 80);
  LCD_Rectangle(5 + a * 80, 46 + b * 80, 29 + a * 80, 61 + b * 80);
  LCD_Rectangle(46 + a * 80, 46 + b * 80, 75 + a * 80, 61 + b * 80);
  LCD_Rectangle(5 + a * 80, 62 + b * 80, 75 + a * 80, 75 + b * 80);

  LCD_setColor(0x0000);
  LCD_setBackColor(color);
  DBG_PrintDEC_XXXgrad(pressure / 10, a * 80 + 15, b * 80 + 30);
  LCD_printXY("p", a * 80 + 30, b * 80 + 46);
}
void If_PrintMetal(uint16_t met)
{
uint16_t color;
uint8_t a, b;

  a = 3;
  b = 2;

  if (met)
    color = Red;
  else
    color = Green;
  
  LCD_setColor(color);
  LCD_Rectangle(5 + a * 80, 5 + b * 80, 75 + a * 80, 45 + b * 80);
  LCD_Rectangle(5 + a * 80, 46 + b * 80, 29 + a * 80, 61 + b * 80);
  LCD_Rectangle(46 + a * 80, 46 + b * 80, 75 + a * 80, 61 + b * 80);
  LCD_Rectangle(5 + a * 80, 62 + b * 80, 75 + a * 80, 75 + b * 80);

  LCD_setColor(0x0000);
  LCD_setBackColor(color);
  LCD_printXY("m", a * 80 + 30, b * 80 + 46);
}
