#ifndef __RS485_H_
#define __RS485_H_

#include "stm32f0xx.h"

#define RS485_RXTX_CTRL1_Pin		GPIO_Pin_11
#define RS485_RX_DATA0_Pin			GPIO_Pin_9
#define RS485_RX_DATA1_Pin			GPIO_Pin_10
#define RS485_RX_DATA0_PinSource9		GPIO_PinSource9
#define RS485_RX_DATA1_PinSource10	GPIO_PinSource10

#define RS485_RXTX_CTRL2_Pin		GPIO_Pin_8
#define RS485_TX_DATA0_Pin			GPIO_Pin_2
#define RS485_TX_DATA1_Pin			GPIO_Pin_3
#define RS485_TX_DATA0_PinSource2	GPIO_PinSource2
#define RS485_TX_DATA1_PinSource3	GPIO_PinSource3

#define RS485_RECV_OPEN					GPIO_ResetBits(GPIOA, RS485_RXTX_CTRL1_Pin)
#define RS485_SEND_OPEN					GPIO_SetBits(GPIOA, RS485_RXTX_CTRL2_Pin)


void UART_485_Configuration(void);
uint8_t RS485_Check(uint8_t *input);
void RS485_SendData(uint8_t *send);
/**********************DMA使用******************************
void RS485_DMA_Init(void);
void RS485_DMA_RcvData(uint8_t *USART1_RxBuffer);							
void RS485_DMA_SendData(uint8_t *USART1_TxBuffer,uint32_t Length);
***********************************************************/
void RS485_Data_Rcv(uint8_t *input,uint8_t *output);

#endif
