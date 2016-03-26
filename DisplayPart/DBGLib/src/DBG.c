/* Includes ------------------------------------------------------------------*/
#include "platform_config.h"
#include "DBG.h"
#ifdef PRINTINGDEVICE_LCD
  #include "LCD.h"
  #include "fonts.h"
#endif
#ifdef PRINTINGDEVICE_UART
  #include "PCComm.h"
#endif

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/    
/* Extern variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void UART_PrintChar(uint8_t data);
void DBG_PrintChar(uint8_t chr);
uint8_t HexCharSet[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

void DBG_Init(void)
{
GPIO_InitTypeDef GPIO_InitStructure;

  /* Enable peripheral clock on APB2 bus */
#if defined STM32F0XX
  RCC_AHBPeriphClockCmd(DBG_RCC_AHBPeriph_PIN | DBG_RCC_AHBPeriph_LED, ENABLE);
#elif defined STM32F10X
  RCC_APB2PeriphClockCmd(DBG_RCC_APB2Periph_PIN | DBG_RCC_APB2Periph_LED, ENABLE);  
#endif

  /* Configure DBG pins */
  GPIO_InitStructure.GPIO_Pin = DBG_PIN1 | DBG_PIN2;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
#if defined STM32F0XX
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
#elif defined STM32F10X
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
#endif
  GPIO_Init(DBG_PIN_PORT, &GPIO_InitStructure);

  /* Configure LEDs pin */
  GPIO_InitStructure.GPIO_Pin = LED1 | LED2;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
#if defined STM32F0XX
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
#elif defined STM32F10X
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
#endif
  GPIO_Init(DBG_LED_PORT, &GPIO_InitStructure);

#ifdef PRINTINGDEVICE_LCD
  LCD_Setup();
  LCD_BackLight(1);
  LCD_setBackColor(Black);
  LCD_setColor(White);
  LCD_setFont((uint8_t*)BigFont);
  LCD_ChangeOrientation(LCD_LANDSCAPE);
  LCD_Clear();
#endif
  
#ifdef PRINTINGDEVICE_UART
  UART_Init();
#endif
}

void DBG_Print(uint8_t *str)
{
  while(*str)
    DBG_PrintChar(*(str++));
}

void DBG_PrintChar(uint8_t chr)
{
  if ((chr < ' ') && (chr != '\n') && (chr != '\r'))
    chr = ' ';
#ifdef PRINTINGDEVICE_UART
    UART_PrintChar(chr);
#endif
#ifdef PRINTINGDEVICE_LCD
    LCD_PrintChar(chr);
#endif
}

void DBG_PrintCR(uint8_t *st)
{
  DBG_Print(st);
  DBG_PrintChar('\n');
  DBG_PrintChar('\r');
}

void DBG_PrintNChars(uint8_t *str, uint8_t N)
{
  while (N-- > 0)
    DBG_PrintChar(*(str++));
}

void DBG_PrintNCharsCR(uint8_t *str, uint8_t N)
{
  while (N-- > 0)
    DBG_PrintChar(*(str++));
  DBG_PrintChar('\n');
  DBG_PrintChar('\r');
}

void DBG_PrintHEX_XX(uint8_t N)
{
  DBG_PrintChar(HexCharSet[(N >> 4) & 0x0F]);
  DBG_PrintChar(HexCharSet[N & 0x0F]);
  DBG_PrintChar('h');
}

void DBG_PrintDEC_XX(uint8_t N)
{
  DBG_PrintChar(HexCharSet[(N / 10) & 0x0F]);
  DBG_PrintChar(HexCharSet[(N % 10) & 0x0F]);
}

void DBG_PrintDEC_XXXX(uint16_t N)
{
  DBG_PrintChar(HexCharSet[((N / 1000) % 10) & 0x0F]);
  DBG_PrintChar(HexCharSet[((N / 100) % 10) & 0x0F]);
  DBG_PrintChar(HexCharSet[((N / 10) % 10) & 0x0F]);
  DBG_PrintChar(HexCharSet[(N % 10) & 0x0F]);
}

void DBG_PrintDEC_XXXgrad(uint16_t N, uint16_t x, uint16_t y)
{
  
  LCD_PrintCharXY(HexCharSet[((N / 100) % 10) & 0x0F], x, y);
  LCD_PrintCharXY(HexCharSet[((N / 10) % 10) & 0x0F], x + BigFont[0], y);
  LCD_PrintCharXY(HexCharSet[(N % 10) & 0x0F], x + 2 * BigFont[0], y);
}

void DBG_PinSet(uint16_t Pin)
{
  GPIO_WriteBit(DBG_PIN_PORT, Pin, Bit_SET);
}

void DBG_PinReset(uint16_t Pin)
{
  GPIO_WriteBit(DBG_PIN_PORT, Pin, Bit_RESET);
}

void DBG_PinTogle(uint16_t Pin)
{
  if (GPIO_ReadOutputDataBit(DBG_PIN_PORT, Pin) == Bit_SET)
    GPIO_WriteBit(DBG_PIN_PORT, Pin, Bit_RESET);
  else
    GPIO_WriteBit(DBG_PIN_PORT, Pin, Bit_SET);
}

void DBG_Led_TurnON(uint16_t Led)
{
  GPIO_WriteBit(DBG_LED_PORT, Led, Bit_SET);
}

void DBG_Led_TurnOFF(uint16_t Led)
{
  GPIO_WriteBit(DBG_LED_PORT, Led, Bit_RESET);
}

void DBG_Led_Togle(uint16_t Led)
{
  if (GPIO_ReadOutputDataBit(DBG_LED_PORT, Led))
    GPIO_WriteBit(DBG_LED_PORT, Led, Bit_RESET);
  else
    GPIO_WriteBit(DBG_LED_PORT, Led, Bit_SET);
}

