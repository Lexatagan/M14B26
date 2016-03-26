/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DBG_H
#define __DBG_H

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define DBG_UART                      USART1
#define DBG_AF_UART                   GPIO_AF_USART1
#define DBG_PINSOURCE_TX              GPIO_PinSource9
#define DBG_PINSOURCE_RX              GPIO_PinSource10

//#define ADEBUG
//#define PIN1_TRANSMITTING
//#define PIN2_IRQPROCESSING

/*#ifdef USE_HY_STM32MINI
  #define PRINTINGDEVICE_LCD
  #define DBG_RCC_APB2Periph_LED    RCC_APB2Periph_GPIOB
  #define DBG_LED_PORT              GPIOB  
  #define LED1                      GPIO_Pin_0  
  #define LED2                      GPIO_Pin_1  

#elif USE_TE_STM32F103
//  #define PRINTINGDEVICE_UART
  #define DBG_RCC_APB2Periph_LED    RCC_APB2Periph_GPIOA
  #define DBG_LED_PORT              GPIOA 
  #define LED1                      GPIO_Pin_4  
  #define LED2                      GPIO_Pin_4  

#elif USE_NK_STM32F103
  #define DBG_RCC_APB2Periph_LED    RCC_APB2Periph_GPIOC
  #define DBG_LED_PORT              GPIOC
  #define LED1                      GPIO_Pin_3  
  #define LED2                      GPIO_Pin_2  

#elif USE_SWTCH10
  #define DBG_RCC_APB2Periph_LED    RCC_APB2Periph_GPIOA
  #define DBG_LED_PORT              GPIOA 
  #define LED1                      GPIO_Pin_2  
  #define LED2                      GPIO_Pin_2  

#endif*/

#define DBG_PIN2_PULSE(P) \
  DBG_PinSet(DBG_PIN2);\
  DBG_PAUSE(P);\
  DBG_PinReset(DBG_PIN2)\

#define DBG_PAUSE(P) \
  i = P;\
  while(i--)\


//#define CompareSpecPacketType(A, B) ((*(A) == *(B)) && (*(A + 1) == *(B + 1)))

/* Exported functions ------------------------------------------------------- */
void DBG_Init(void);
void DBG_PinSet(uint16_t Pin);
void DBG_PinReset(uint16_t Pin);
void DBG_PinTogle(uint16_t Pin);
void DBG_Print(uint8_t *st);
void DBG_PrintCR(uint8_t *st);
void DBG_PrintNChars(uint8_t *str, uint8_t N);
void DBG_PrintNCharsCR(uint8_t *str, uint8_t N);
void DBG_PrintHEX_XX(uint8_t N);
void DBG_PrintDEC_XX(uint8_t N);
void DBG_PrintDEC_XXXX(uint16_t N);
void DBG_Led_TurnON(uint16_t Led);
void DBG_Led_TurnOFF(uint16_t Led);
void DBG_Led_Togle(uint16_t Led);
void DBG_PrintDEC_XXXgrad(uint16_t N, uint16_t x, uint16_t y);

/* External variables --------------------------------------------------------*/
#endif   /* __DBG_H */
