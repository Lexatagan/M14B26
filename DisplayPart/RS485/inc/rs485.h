/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __RS485_H
#define __RS485_H

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/
#define RS485_MESSAGE_HEAD                      0x2454

/* Exported functions ------------------------------------------------------- */
void rs485_init(void);
void rs485_receive(uint8_t chr);

/* External variables --------------------------------------------------------*/
#endif   /* __RS485_H */
