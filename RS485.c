#include "RS485.h"
#include "global_var.h"

DMA_InitTypeDef *DMA_InitStructure;

/*******************************************************************************
* Name  : UART1_Configuration for RS485
* Deion        : Configures the uart1
* Input                    : None
* Output                 : None
* Return                 : None
*******************************************************************************/
void UART_485_Configuration(void)
{
	
	USART_InitTypeDef UART_InitStructure;
		
#ifdef USE_OVERSAMPLING_8
  #define OVERSAMPLING_VALUE 0x08
  USART_OverSampling8Cmd(USART1, ENABLE);
#else 
  #define OVERSAMPLING_VALUE 0x10
#endif

	/*串口时钟使能， GPIO 时钟使能*/
	USART_DeInit(USART1);
	
	/*fill the structure data*/
	USART_StructInit(&UART_InitStructure);
	
	/* Configure USART1 basic and asynchronous paramters */
	USART_Init(USART1,&UART_InitStructure);

	/* Enable driver enable mode */  
  USART_DECmd(USART1, ENABLE);
  
  /* Configure DE assertion time */
  USART_SetDEAssertionTime(USART1, OVERSAMPLING_VALUE);
  
  /* Configure DE deassertion time */  
  USART_SetDEDeassertionTime(USART1, OVERSAMPLING_VALUE);
  
  /* Configure polarity of DE */
  USART_DEPolarityConfig(USART1, USART_DEPolarity_Low);//USART_DEPolarity_High 是发送，USART_DEPolarity_Low 是接收
	
	USART_Cmd(USART1, ENABLE); //使能USART1	
	
	while(USART_GetFlagStatus(USART1, USART_FLAG_REACK) == RESET)
  {}  
  while(USART_GetFlagStatus(USART1, USART_FLAG_TEACK) == RESET)
  {} 
  
  /* Wait until TXE=1 */
  while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
  {}	
	
	/* Enable USART1 Receive interrupts */
  //USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	
	USART_ClearFlag(USART1, USART_FLAG_TC); 
	
  /* Enable USART1 DMA TX request */
  USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);//设置发送标志位
  /* Enable USART1 DMA RX request */
  USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);//设置接收标志位
}

void RS485_Init(void)
{
	DMA_StructInit(DMA_InitStructure);
	DMA_DeInit(DMA1_Channel4);
  DMA_InitStructure->DMA_DIR = DMA_DIR_PeripheralDST;
	DMA_Init(DMA1_Channel4,DMA_InitStructure);
	
	DMA_DeInit(DMA1_Channel5);
  DMA_InitStructure->DMA_DIR = DMA_DIR_PeripheralSRC;
  DMA_InitStructure->DMA_BufferSize = 10;//Max receive data length
	DMA_Init(DMA1_Channel5,DMA_InitStructure);
}
//485 DMA数据发送
void RS485RcvData(uint8_t *USART1_RxBuffer)
{
	USART_DEPolarityConfig(USART1, USART_DEPolarity_High);
  DMA_InitStructure->DMA_MemoryBaseAddr = (uint32_t)USART1_RxBuffer;
  /* Enable DMA1 Channel4 transmit complete interrupt */
  DMA_ITConfig(DMA1_Channel4, DMA_IT_TC, ENABLE);
  /* Enable DMA1 Channel4 */
  DMA_Cmd(DMA1_Channel4, ENABLE);
}

//485 DMA数据接收
void RS485SendData(uint8_t *USART1_TxBuffer,uint32_t Length)
{
	USART_DEPolarityConfig(USART1, USART_DEPolarity_Low);
  DMA_InitStructure->DMA_MemoryBaseAddr = (uint32_t)USART1_TxBuffer;
  DMA_InitStructure->DMA_BufferSize = Length;
  /* Enable DMA1 Channel5 transmit complete interrupt */
  DMA_ITConfig(DMA1_Channel5, DMA_IT_TC, ENABLE);
  /* Enable DMA1 Channel5 */
  DMA_Cmd(DMA1_Channel5, ENABLE);
}
