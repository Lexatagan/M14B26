/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"
#include "rs485.h"

/* Private typedef -----------------------------------------------------------*/

UART_HandleTypeDef huart1;

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

  huart1.Instance = USART1;
  huart1.Init.BaudRate = 9600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  HAL_UART_Init(&huart1);

  /* DMA interrupt init */
  HAL_NVIC_SetPriority(USART1_IRQn, 2, 0);
  HAL_NVIC_EnableIRQ(USART1_IRQn);
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