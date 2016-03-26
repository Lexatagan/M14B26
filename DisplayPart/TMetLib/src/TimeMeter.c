/* Includes ------------------------------------------------------------------*/
#include "platform_config.h"
#include "TimeMeter.h"

/* Private typedef -----------------------------------------------------------*/
//typedef enum {FALSE = 0, TRUE = !FALSE} bool;
typedef void TExecutionProcedure (void);
typedef struct
{
  uint32_t TimeElapsed;
  TExecutionProcedure *ExecutionProcedure;
} Tmet_VirtualTimer;

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/ 
volatile bool TimerBusy;
Tmet_VirtualTimer VirtTimer[TMET_VIRTTIMER_QUANTITY];
/* Extern variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void TMet_ResetTimerBusy(void);

void TMet_Init(void)
{
TIM_TimeBaseInitTypeDef    TIM_TimeBaseStructure;
NVIC_InitTypeDef           NVIC_InitStructure;
RCC_ClocksTypeDef Clk;
uint8_t i;

  for (i = 0; i < TMET_VIRTTIMER_QUANTITY; i++)
    VirtTimer[i].TimeElapsed = 0;

  RCC_APB1PeriphClockCmd(TMET_RCC, ENABLE);
  
  RCC_GetClocksFreq(&Clk);

  /* Time base configuration */
  TIM_TimeBaseStructure.TIM_Period = TMET_1MS_PERIOD - 1;           
  TIM_TimeBaseStructure.TIM_Prescaler = (Clk.SYSCLK_Frequency) / 1000000 - 1;   //Corresponds to 1us timer tick
  TIM_TimeBaseStructure.TIM_ClockDivision = 0x00;    
  TIM_TimeBaseStructure.TIM_CounterMode =  TIM_CounterMode_Up;  
  
  TIM_TimeBaseInit(TMET_TIM, &TIM_TimeBaseStructure);

  TIM_ARRPreloadConfig(TMET_TIM, ENABLE);
  
  TIM_ITConfig(TMET_TIM, TIM_IT_Update, ENABLE);
  /* TMET_TIM enable counter -------------------------------------------------*/
  TIM_Cmd(TMET_TIM, ENABLE);
  
  /* Enable IRQ Channel ------------------------------------------------------*/
  NVIC_InitStructure.NVIC_IRQChannel = TMET_IRQChannel;
#if defined STM32F0XX
  NVIC_InitStructure.NVIC_IRQChannelPriority = 0x00;
#elif defined STM32F10X
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
#endif
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

void TMet_Delay(uint32_t Delay)
{
  TMet_DelayedExecution(TMet_ResetTimerBusy, Delay);
  TimerBusy = TRUE;
  while(TimerBusy);
}

void TMet_IRQCallBack(void)
{
uint8_t i;
  for (i = 0; i < TMET_VIRTTIMER_QUANTITY; i++)
    if (VirtTimer[i].TimeElapsed != 0) 
    {
      VirtTimer[i].TimeElapsed--;
      if (VirtTimer[i].TimeElapsed == 0)
        VirtTimer[i].ExecutionProcedure();
    }
}

uint8_t TMet_DelayedExecution(void ExecutionProcedure(void), uint32_t Delay)
{
uint8_t i;
  for (i = 0; i < TMET_VIRTTIMER_QUANTITY; i++)
    if (VirtTimer[i].TimeElapsed == 0) 
    {
      VirtTimer[i].ExecutionProcedure = ExecutionProcedure;
      VirtTimer[i].TimeElapsed = Delay;
      return 1;
    }
  return 0;
}

void TMet_ResetTimerBusy(void)
{
  TimerBusy = FALSE;
}

/*void TMet_StartCapture(uint16_t Presc)
{
  TIM_PrescalerConfig(TMET_TIM, Presc, TIM_PSCReloadMode_Update);
  TIM_GenerateEvent(TMET_TIM, TIM_EventSource_Update);
}

uint16_t TMet_StopCapture(void)
{
  return TIM_GetCounter(TMET_TIM);
}*/

/*void TMet_StartCompare(uint16_t Presc, uint16_t Time)
{
  TIM_PrescalerConfig(TMET_TIM, Presc, TIM_PSCReloadMode_Update);
  TIM_SetAutoreload(TMET_TIM, Time);
  TIM_GenerateEvent(TMET_TIM, TIM_EventSource_Update);
  TIM_ClearITPendingBit(TMET_TIM, TIM_IT_Update);
  TIM_ITConfig(TMET_TIM, TIM_IT_Update, ENABLE);
  TimerBusy = TRUE;
}*/

/*void TMet_Wait_ms(uint16_t Time)
{
uint16_t Presc;
RCC_ClocksTypeDef Clk;

  RCC_GetClocksFreq(&Clk);
  Presc = (Clk.SYSCLK_Frequency) / 1000 / 10 - 1;                                   //Presc have to correspond to 0.1 ms clock period of timer
  TMet_StartCompare(Presc, Time * 10);
}*/

/*void TMet_StopCompare(uint16_t Presc, uint16_t Time)
{
  TIM_ITConfig(TMET_TIM, TIM_IT_Update, DISABLE);
}*/
