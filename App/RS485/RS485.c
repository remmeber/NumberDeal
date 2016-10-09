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
  USART²ÎÊý³õÊ¼»¯:  ²¨ÌØÂÊ     ´«ÊäÎ»Êý   Í£Ö¹Î»Êý  Ð£ÑéÎ»Êý
                    9600         8         1        0(NO)
  *******************************************************************/
  USART_InitStructure.USART_BaudRate = 9600;                       //Éè¶¨´«ÊäËÙÂÊ
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;        //Éè¶¨´«ÊäÊý¾ÝÎ»Êý
  USART_InitStructure.USART_StopBits = USART_StopBits_1;             //Éè¶¨Í£Ö¹Î»¸öÊý
  USART_InitStructure.USART_Parity = USART_Parity_No ;               //²»ÓÃÐ£ÑéÎ»
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//²»ÓÃÁ÷Á¿¿ØÖÆ
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;    //Ê¹ÓÃ½ÓÊÕºÍ·¢ËÍ¹¦ÄÜ
  USART_Init(USART1, &USART_InitStructure);                          //³õÊ¼»¯USART1

  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);                     //Ê¹ÄÜUSART1½ÓÊÕÖÐ¶Ï

  USART_Cmd(USART1, ENABLE);                                         //Ê¹ÄÜUSART1
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

uint8_t RS485_Check(uint8_t *input)
{
	uint16_t checkSum = 0;
	uint8_t i = 0;
	if (input == 0)
        return 0;
    if (input[0] == 0x42)
    {
        for (; i <= 9; i++)
            checkSum += input[i];
    }
    if ((uint8_t)(checkSum >> 8) == input[11] && (uint8_t)checkSum == input[10])
        return 1;
    else
        return 0;
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

void RS485_Data_Rcv(uint8_t *input,uint8_t *output)
{
	int i;
	for(i=0;i<12;i++)
	{
		output[i] = input[i];
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
