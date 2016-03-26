/******************** (C) COPYRIGHT 2014 LX ltd. ******************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __PLATFORM_CONFIG_H
#define __PLATFORM_CONFIG_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Define the STM32F10x hardware depending on the used board */

/*typedef enum
{
  FALSE = 0, TRUE  = !FALSE
}
bool;*/

#define RCC_PLLMUL                RCC_PLLMul_9

//******************************************************************************
//*DBG                                                                         *
//******************************************************************************
#define PRINTINGDEVICE_LCD
#define DBG_RCC_APB2Periph_LED        RCC_APB2Periph_GPIOB
#define DBG_LED_PORT                  GPIOB 
#define LED1                          GPIO_Pin_0  
#define LED2                          GPIO_Pin_1  
#define DBG_RCC_APB2Periph_PIN        RCC_APB2Periph_GPIOC
#define DBG_PIN_PORT                  GPIOC
#define DBG_PIN1                      GPIO_Pin_10
#define DBG_PIN2                      GPIO_Pin_11

#define UART_RCC_Periph_UART                            RCC_APB2Periph_USART1
#define UART                                            USART1
#define UART_RCC_Periph_PORT                            RCC_APB2Periph_GPIOA
#define UART_PORT                                       GPIOA
#define UART_TX                                         GPIO_Pin_9
#define UART_RX                                         GPIO_Pin_10

#define UART_IRQChannel                                 USART1_IRQn

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* __PLATFORM_CONFIG_H */

/******************* (C) COPYRIGHT 2014 LX ltd. **********END OF FILE**********/
