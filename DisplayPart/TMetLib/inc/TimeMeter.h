/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TIMEMETER_H
#define __TIMEMETER_H

/* This module using TIM4 and GPIOB Pin6, 7 as PWM outputs*/

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/
#define TMET_TIM                 TIM3                                           //General purpose timer
#define TMET_RCC                 RCC_APB1Periph_TIM3
#define TMET_IRQChannel          TIM3_IRQn                                      //Don't forget to change destination of IRQ_Callback
#define TMET_1MS_PERIOD          1000                                           //Corresponds to 1ms period of timer
#define TMET_VIRTTIMER_QUANTITY  2
/* Exported functions ------------------------------------------------------- */
void TMet_Init(void);
void TMet_IRQCallBack(void);
void TMet_Delay(uint32_t Delay);                                                //Hold processor for Delay ms
uint8_t TMet_DelayedExecution(void ExecutionProcedure(void), uint32_t Delay);   //Execute ExternProcedure() after Delay ms. Only TMET_VIRTTIMER_QUANTITY procedures can waiting at the same time

//void TMet_StartCapture(u16 Presc);
//u16 TMet_StopCapture(void);
//void TMet_StartCompare(u16 Presc, u16 Time);
//void TMet_StopCompare(u16 Presc, u16 Time);
/* External variables --------------------------------------------------------*/
//extern volatile bool TimerBusy;
#endif   /* __TIMEMETER_H */
