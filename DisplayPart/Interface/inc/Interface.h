/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __INTERFACE_H
#define __INTERFACE_H

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/
#define GREEN_THRESHOLD                         1000
#define RED_THRESHOLD                           2000

/* Exported functions ------------------------------------------------------- */
void If_PrintCylCell(uint8_t N, uint16_t temp);
void If_PrintOilTemp(uint16_t temp);
void If_PrintOilPressure(uint16_t pressure);
void If_PrintMetal(uint16_t met);

/* External variables --------------------------------------------------------*/
#endif   /* __TIMEMETER_H */
