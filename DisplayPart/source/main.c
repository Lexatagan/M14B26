/******************** (C) COPYRIGHT 2014 LX ltd. ******************************/

#include "platform_config.h"
#include "hw_config.h"
#include "TimeMeter.h"
#include "Interface.h"
#include "DBG.h"
#include "rs485.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
u8 Tst = 1;
uint16_t cl[9];
uint16_t ot;
uint16_t op;
uint16_t mt;
/* Extern variables ----------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : main.
* Description    : Main routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/

int main()
{
u32 R;
uint16_t a, b;
  Set_System();
  TMet_Init();
  DBG_Init();
  rs485_init();
  while(1)
  {
    TMet_Delay(100);
    for (a = 0; a < 3; a++)
      for (b = 0; b < 3; b++)
      {
        If_PrintCylCell(a + b * 3, cl[a + b * 3]);
      }
    If_PrintOilTemp(ot);
    If_PrintOilPressure(op);
    If_PrintMetal(mt);
  };
}

#ifdef  DEBUG
/*******************************************************************************
* Function Name  : assert_failed
* Description    : Reports the name of the source file and the source line number
*                  where the assert_param error has occurred.
* Input          : - file: pointer to the source file name
*                  - line: assert_param error line source number
* Output         : None
* Return         : None
*******************************************************************************/
void assert_failed(u8* file, u32 line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {}
}
#endif

/******************* (C) COPYRIGHT 2014 LX ltd. **********END OF FILE**********/
