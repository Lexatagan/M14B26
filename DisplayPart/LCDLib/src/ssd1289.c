#include "platform_config.h"
#include "ssd1289.h"
#include "LCD.h"


void Drv_Delay(u16 nCount);

//static u16 Channel2Pulse = 1000;

//static gl_sFONT *LCD_Currentfonts;

#define LCD_REG              (*((volatile unsigned short *) 0x60000000))
/*In case of a 16-bit external memory width, the FSMC will internally 
use HADDR[25:1] to generate the address for external memory FSMC_A[24:0].
Whatever the external memory width (16-bit or 8-bit), FSMC_A[0] should be
connected to external memory */
#define LCD_RAM              (*((volatile unsigned short *) 0x60020000))

void Drv_CtrlLinesConfig(void)
{
GPIO_InitTypeDef GPIO_InitStructure;

  /* Enable FSMC, GPIOD, GPIOE and AFIO clocks */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE | RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
                        
  /* PE7(D4), PE8(D5), P09(D6), PE10(D7), PE11(D8), PE12(D9),
     PE13(D10), PE14(D11), PE15(D12) */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 |
                                 GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | 
                                 GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOE, &GPIO_InitStructure);

  /* PD0(D2), PD1(D3), PD4(RD), PD5(WR), PD7(CS), PD8(D13), PD9(D14),
     PD10(D15), PD11(RS) PD14(D0) PD15(D1) */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_7 | 
                                 GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | 
                                 GPIO_Pin_14 | GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  
  /* PB5 (BL)*/ 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void Drv_FSMCConfig(void)
{  
FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
FSMC_NORSRAMTimingInitTypeDef FSMC_NORSRAMTimingInitStructure;
  
  FSMC_NORSRAMTimingInitStructure.FSMC_AddressSetupTime = 5;//1;   
  FSMC_NORSRAMTimingInitStructure.FSMC_AddressHoldTime = 0;	   
  FSMC_NORSRAMTimingInitStructure.FSMC_DataSetupTime = 10;//1;	   
  FSMC_NORSRAMTimingInitStructure.FSMC_BusTurnAroundDuration = 0x00;	
  FSMC_NORSRAMTimingInitStructure.FSMC_CLKDivision = 0x00;
  FSMC_NORSRAMTimingInitStructure.FSMC_DataLatency = 0x00;
  FSMC_NORSRAMTimingInitStructure.FSMC_AccessMode = FSMC_AccessMode_A;	
  FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &FSMC_NORSRAMTimingInitStructure;	  
  
  FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM1;
  FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;
  FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_SRAM;
  FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;
  FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
  FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
  FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;
  FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
  FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;
  FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
  FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;
  FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
  FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &FSMC_NORSRAMTimingInitStructure;
	
  FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure); 
	
  /* Enable FSMC Bank1_SRAM Bank */
  FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE); 
}

void Drv_Init()
{
  u16 DeviceCode = Drv_ReadReg(0x0000);
  if(DeviceCode==0x8989)
  {
    Drv_WriteReg(0x0000,0x0001);    Drv_Delay(5);
    Drv_WriteReg(0x0003,0xA8A4);    Drv_Delay(5);   
    Drv_WriteReg(0x000C,0x0000);    Drv_Delay(5);   
    Drv_WriteReg(0x000D,0x080C);    Drv_Delay(5);   
    Drv_WriteReg(0x000E,0x2B00);    Drv_Delay(5);   
    Drv_WriteReg(0x001E,0x00B0);    Drv_Delay(5);   
    Drv_WriteReg(0x0001,0x2B3F);    Drv_Delay(5);
    Drv_WriteReg(0x0002,0x0600);    Drv_Delay(5);
    Drv_WriteReg(0x0010,0x0000);    Drv_Delay(5);
    Drv_WriteReg(0x0011,0x6030);    Drv_Delay(5);
    Drv_WriteReg(0x0005,0x0000);    Drv_Delay(5);
    Drv_WriteReg(0x0006,0x0000);    Drv_Delay(5);
    Drv_WriteReg(0x0016,0xEF1C);    Drv_Delay(5);
    Drv_WriteReg(0x0017,0x0003);    Drv_Delay(5);
    Drv_WriteReg(0x0007,0x0333);    Drv_Delay(5);         
    Drv_WriteReg(0x000B,0x0000);    Drv_Delay(5);
    Drv_WriteReg(0x000F,0x0000);    Drv_Delay(5);
    Drv_WriteReg(0x0041,0x0000);    Drv_Delay(5);
    Drv_WriteReg(0x0042,0x0000);    Drv_Delay(5);
    Drv_WriteReg(0x0048,0x0000);    Drv_Delay(5);
    Drv_WriteReg(0x0049,0x013F);    Drv_Delay(5);
    Drv_WriteReg(0x004A,0x0000);    Drv_Delay(5);
    Drv_WriteReg(0x004B,0x0000);    Drv_Delay(5);
    Drv_WriteReg(0x0044,0xEF00);    Drv_Delay(5);
    Drv_WriteReg(0x0045,0x0000);    Drv_Delay(5);
    Drv_WriteReg(0x0046,0x013F);    Drv_Delay(5);
    Drv_WriteReg(0x0030,0x0707);    Drv_Delay(5);
    Drv_WriteReg(0x0031,0x0204);    Drv_Delay(5);
    Drv_WriteReg(0x0032,0x0204);    Drv_Delay(5);
    Drv_WriteReg(0x0033,0x0502);    Drv_Delay(5);
    Drv_WriteReg(0x0034,0x0507);    Drv_Delay(5);
    Drv_WriteReg(0x0035,0x0204);    Drv_Delay(5);
    Drv_WriteReg(0x0036,0x0204);    Drv_Delay(5);
    Drv_WriteReg(0x0037,0x0502);    Drv_Delay(5);
    Drv_WriteReg(0x003A,0x0302);    Drv_Delay(5);
    Drv_WriteReg(0x003B,0x0302);    Drv_Delay(5);
    Drv_WriteReg(0x0023,0x0000);    Drv_Delay(5);
    Drv_WriteReg(0x0024,0x0000);    Drv_Delay(5);
    Drv_WriteReg(0x0025,0x8000);    Drv_Delay(5);
    Drv_WriteReg(0x004f,0);
    Drv_WriteReg(0x004e,0);
  }
  Drv_Delay(5);  /* delay 5 ms */		
}

void Drv_WriteReg(u8 LCD_Reg, u16 LCD_RegValue)
{
  LCD_REG = LCD_Reg;
  LCD_RAM = LCD_RegValue;
}

u16 Drv_ReadReg(u8 LCD_Reg)
{
  LCD_REG = LCD_Reg;
  return (LCD_RAM);
}

void Drv_WriteRAM_Prepare(void)
{
  LCD_REG = R34;
}

inline void Drv_WriteRAM(u16 RGB_Code)					 
{
  LCD_RAM = RGB_Code;
}

u16 Drv_ReadRAM(void)
{
  volatile u16 dummy; 
  LCD_REG = R34; /* Select GRAM Reg */
  dummy = LCD_RAM; 
  
  return LCD_RAM;
}

void Drv_DummyReadRAM(void)
{
  volatile u16 dummy; 
  dummy = LCD_RAM; 
}

void Drv_Delay(u16 nCount)
{
 u16 TimingDelay; 
 while(nCount--)
   for(TimingDelay=0;TimingDelay<10000;TimingDelay++);
}

void Drv_PrintPixel(u16 color)
{
  Drv_WriteRAM( color );      
}

void Drv_SetCursor(u16 x, u16 y)
{
  Drv_WriteReg(0x4e, x);
  Drv_WriteReg(0x4f, y);
  Drv_WriteRAM_Prepare(); 
}

/*void Drv_PowerOn(void)
{
  //
}

void Drv_DisplayOn(void)
{
  u16 dummy = LCD_ReadReg(0x0007);
  dummy |= 0x0033;
  LCD_WriteReg(0x0007, dummy);
  LCD_SSDBackLight(100);
}

void Drv_DisplayOff(void)
{
  u16 dummy = LCD_ReadReg(0x0007);
  dummy &= ~0x0033;
  LCD_WriteReg(0x0007, dummy);
  LCD_SSDBackLight(0);
}*/

void Drv_BackLight(u8 percent)
{
  if( percent > 0 && percent <= 100)
    GPIO_SetBits(GPIOB, GPIO_Pin_5);
  else
    GPIO_ResetBits(GPIOB, GPIO_Pin_5);
}
