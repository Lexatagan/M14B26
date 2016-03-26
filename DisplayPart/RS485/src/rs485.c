/* Includes ------------------------------------------------------------------*/
#include "platform_config.h"
#include "rs485.h"

/* Private typedef -----------------------------------------------------------*/
#pragma pack(1)  
typedef struct
{
  uint16_t MsgHead;
  uint16_t MsgSynch;
  uint8_t* index;
  uint16_t compleated;
  uint16_t error;
  uint16_t Cyl1Temperature;
  uint16_t Cyl2Temperature;
  uint16_t Cyl3Temperature;
  uint16_t Cyl4Temperature;
  uint16_t Cyl5Temperature;
  uint16_t Cyl6Temperature;
  uint16_t Cyl7Temperature;
  uint16_t Cyl8Temperature;
  uint16_t Cyl9Temperature;
  uint16_t OilTemperature;
  uint16_t OilPressure;
  uint16_t Metal;
  uint16_t crc;
} S1Message;
#pragma pack()

typedef struct
{
  uint8_t comma1;
  uint8_t Cyl1Temperature;
  uint8_t comma2;
  uint8_t Cyl2Temperature;
  uint8_t comma3;
  uint8_t Cyl3Temperature;
  uint8_t comma4;
  uint8_t Cyl4Temperature;
  uint8_t comma5;
  uint8_t Cyl5Temperature;
  uint8_t comma6;
  uint8_t Cyl6Temperature;
  uint8_t comma7;
  uint8_t Cyl7Temperature;
  uint8_t comma8;
  uint8_t Cyl8Temperature;
  uint8_t comma9;
  uint8_t Cyl9Temperature;
  uint8_t comma10;
  uint8_t OilTemperature;
  uint8_t comma11;
  uint8_t OilPressure;
  uint8_t comma12;
  uint8_t Metal;
  uint8_t comma13;
  uint8_t crc;
  uint8_t rn;
} IntStruct;

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
S1Message data;
uint16_t MsgHead = 0; 
uint8_t MsgSynch = 0;
uint16_t rs485error = 0;
/* Extern variables ----------------------------------------------------------*/
extern uint16_t cl[9];
extern uint16_t ot;
extern uint16_t op;
extern uint16_t mt;
//IntStruct Msg;
/* Private function prototypes -----------------------------------------------*/

void rs485_init(void)
{
GPIO_InitTypeDef     GPIO_InitStructure;

  /*Enable USART clock*/
  RCC_APB2PeriphClockCmd(UART_RCC_Periph_UART, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
  RCC_APB2PeriphClockCmd(UART_RCC_Periph_PORT, ENABLE);

  /*Configurate TX*/
  GPIO_InitStructure.GPIO_Pin = UART_TX;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;                              
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(UART_PORT, &GPIO_InitStructure);
  
  /*Configurate RX*/
  GPIO_InitStructure.GPIO_Pin = UART_RX;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;                      
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(UART_PORT, &GPIO_InitStructure);
  
USART_InitTypeDef    USART_InitStructure;
  USART_InitStructure.USART_BaudRate = 9600;                                   //Скорость обмена 38400 бод
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;                   //Длина слова 8 бит
  USART_InitStructure.USART_StopBits = USART_StopBits_1;                        //1 стоп-бит
  USART_InitStructure.USART_Parity = USART_Parity_No ;                          //Без проверки четности
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //Без аппаратного контроля
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;               //Включен передатчик UART
  USART_Init(UART, &USART_InitStructure);                                       //Заданные настройки сохраняем в регистрах UART

  USART_ClearITPendingBit(UART, USART_IT_RXNE);
  USART_ClearITPendingBit(UART, USART_IT_TC);
  USART_ITConfig(UART, USART_IT_RXNE, ENABLE);                                  //Прерывание по приему

  /* Enable UART IRQ Channel -------------------------------------------------*/
NVIC_InitTypeDef     NVIC_InitStructure;

  NVIC_InitStructure.NVIC_IRQChannel = UART_IRQChannel;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;

  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  USART_Cmd(UART, ENABLE); //Включаем UART
}

void rs485_receive(uint8_t chr)
{

  data.MsgHead = (data.MsgHead << 8) + chr;
  if (data.MsgSynch)
  {
    *(data.index) = chr;
    if (data.index == (uint8_t*)&(data.Metal) + 1)
    {
      data.compleated = 1;
      data.MsgSynch = 0;
    }
    else
      data.index++;
  }
  if (data.compleated)
  {
    data.compleated = 0;
    cl[0] = data.Cyl1Temperature;
    cl[1] = data.Cyl2Temperature;
    cl[2] = data.Cyl3Temperature;
    cl[3] = data.Cyl4Temperature;
    cl[4] = data.Cyl5Temperature;
    cl[5] = data.Cyl6Temperature;
    cl[6] = data.Cyl7Temperature;
    cl[7] = data.Cyl8Temperature;
    cl[8] = data.Cyl9Temperature;
    ot = data.OilTemperature;
    op = data.OilPressure;
    mt = data.Metal;
  }    
  if (data.MsgHead == RS485_MESSAGE_HEAD)
  {
    if (data.MsgSynch == 1)
      data.error++;
    data.MsgSynch = 1;
    data.index = (uint8_t*)(&(data.Cyl1Temperature));
  }  
}