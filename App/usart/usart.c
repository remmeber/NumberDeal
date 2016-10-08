#include "usart.h"


/************************************************
author		  ruanhugang
function    USART_Configuration
description Usart configuration
arg(input)	null
return		  null
out(outpur)	null
*************************************************/
void USART_Configuration(void)
{
  USART_InitTypeDef USART_InitStructure;

  /******************************************************************
  USART参数初始化： 波特率     传输位数     停止位数    校验位数 
                    9600         8         	1         0(NO)
  *******************************************************************/
  USART_InitStructure.USART_BaudRate = 9600;                     					        //设定传输速率
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;       				      //设定传输数据位数
  USART_InitStructure.USART_StopBits = USART_StopBits_1;            				      //设定停止位个数
  USART_InitStructure.USART_Parity = USART_Parity_No ;              			 	      //不用校验位
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//不用流量控制
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;    				      //使用接收和发送功能
  USART_Init(USART1, &USART_InitStructure);                          				      //初始化USART1

  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);                     				      //使能USART1接收中断

  USART_Cmd(USART1, ENABLE);                                         				      //使能USART1
}

/************************************************
author		  ruanhugang
function 	  USART_Initializes
description Initialize the usart
arg(input)	null
return		  null
out(outpur)	null
*************************************************/
void USART_Initializes(void)
{
  USART_Configuration();
}
