#include "RS485.h"
#include "global_var.h"

DMA_InitTypeDef DMA_InitStructure;

/*******************************************************************************
* Name  : UART1_Configuration for RS485
* Deion        : Configures the uart1
* Input                    : None
* Output                 : None
* Return                 : None
*******************************************************************************/
void UART_485_Configuration(void)
{
	
	USART_InitTypeDef USART_InitStructure;
  /******************************************************************
  USART参数初始化： 波特率     传输位数     停止位数    校验位数 
                    9600         8         	  1        	 0(NO)
  *******************************************************************/
  USART_InitStructure.USART_BaudRate = 9600;                     						//设定传输速率
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;       				    //设定传输数据位数
  USART_InitStructure.USART_StopBits = USART_StopBits_1;            				    //设定停止位个数
  USART_InitStructure.USART_Parity = USART_Parity_No ;              			 	    //不用校验位
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;		//不用流量控制
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;    				    //使用接收和发送功能
  USART_Init(USART1, &USART_InitStructure);                          				    //初始化USART1
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);                     				    //使能USART1接收中断

  USART_Cmd(USART1, ENABLE);                                         				     //使能USART1
	/* Enable driver enable mode */  
  //USART_DECmd(USART1, ENABLE);

  /* Configure DE assertion time */
  //USART_SetDEAssertionTime(USART1, OVERSAMPLING_VALUE);  
  
  /* Configure DE deassertion time */  
  //USART_SetDEDeassertionTime(USART1, OVERSAMPLING_VALUE);
  
  /* Configure polarity of DE */
  //USART_DEPolarityConfig(USART1, USART_DEPolarity_Low);//USART_DEPolarity_High 是发送，USART_DEPolarity_Low 是接收



	/* Enable USART3 Receive and Transmit interrupts */

	//while(USART_GetFlagStatus(USART1, USART_FLAG_REACK) == RESET)
	//{
	//}
  //while(USART_GetFlagStatus(USART1, USART_FLAG_TEACK) == RESET)
  //{} 
  
  /* Wait until TXE=1 */
  //while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
  //{
  //}   
	
	
  /* Enable USART1 DMA RX request */
  //USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);//设置接收标志位
	
	
	/* Configure USART2 basic and asynchronous paramters */
	//USART_Init(USART2,&USART_InitStructure);
	/* Enable driver enable mode */  
  //USART_DECmd(USART2, ENABLE);
  /* Configure DE assertion time */
  //USART_SetDEAssertionTime(USART2, OVERSAMPLING_VALUE);
  /* Configure DE deassertion time */  
	//USART_SetDEDeassertionTime(USART2, OVERSAMPLING_VALUE);
  /* Configure polarity of DE */
  //USART_DEPolarityConfig(USART2, USART_DEPolarity_High);//USART_DEPolarity_High 是发送，USART_DEPolarity_Low 是接收

	//USART_Cmd(USART2, ENABLE); //使能USART2	
  
	//while(USART_GetFlagStatus(USART2, USART_FLAG_REACK) == RESET)
	//{
	//}
  //while(USART_GetFlagStatus(USART2, USART_FLAG_TEACK) == RESET)
 // {} 
  
  /* Wait until TXE=1 */
 // while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET)
 // {
//  }

  /* Enable USART2 DMA TX request */
  //USART_DMACmd(USART2, USART_DMAReq_Tx, ENABLE);//设置发送标志位
}

/*DMA 初始化*/
void RS485_DMA_Init(void)
{
	DMA_DeInit(DMA1_Channel5);
  /* Initialize the DMA_PeripheralBaseAddr member */
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART1->RDR;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
  DMA_InitStructure.DMA_BufferSize = 10;//Max receive data length
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)RS485_RCV_DATA;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  /* Enable DMA1 Channel5 transmit complete interrupt*/ 
  /* Enable DMA1 Channel5 */
	DMA_Init(DMA1_Channel5,&DMA_InitStructure); 
  DMA_ITConfig(DMA1_Channel5, DMA_IT_TC, ENABLE);
	DMA_ClearFlag(DMA1_FLAG_TC5);
	
}
/*485 DMA1数据接收*/
void RS485_DMA_RcvData(uint8_t *USART1_RxBuffer)
{
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)USART1_RxBuffer;
  /* Enable DMA1 Channel5 transmit complete interrupt 
  DMA_ITConfig(DMA1_Channel5, DMA_IT_TC, ENABLE);*/
  /* Enable DMA1 Channel5 */
  DMA_Cmd(DMA1_Channel5, ENABLE);
}

void RS485_Data_Rcv(void)
{
	int i;
	for(i=0;i<13;i++)
	{
		RS485_RCV_DATA[i] = RS485_Rcv_temp[i];
	}
}

/*485 DMA2数据发送*/
void RS485_DMA_SendData(uint8_t *USART2_TxBuffer,uint32_t Length)
{
	GPIO_SetBits(GPIOA,GPIO_Pin_11);
	DMA_DeInit(DMA1_Channel4);
	DMA_StructInit(&DMA_InitStructure);
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART1->TDR;
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)USART2_TxBuffer;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
  DMA_InitStructure.DMA_BufferSize = Length;
	DMA_Init(DMA1_Channel4,&DMA_InitStructure);
  /* Enable DMA1 Channel5 transmit complete interrupt */
  DMA_ITConfig(DMA1_Channel4, DMA_IT_TC, ENABLE);
  /* Enable DMA1 Channel5 */
  DMA_Cmd(DMA1_Channel4, ENABLE);
}
