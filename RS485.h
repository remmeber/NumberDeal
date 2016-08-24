#ifndef __RS485_H_
#define __RS485_H_

#include "stm32f0xx.h"

void UART_485_Configuration(void);
void RS485_Init(void);
void RS485RcvData(uint8_t *USART1_RxBuffer);
void RS485SendData(uint8_t *USART1_TxBuffer,uint32_t Length);

#endif
