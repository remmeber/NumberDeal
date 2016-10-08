
#include "basic.h"
#include "wiegand.h"
#include "RS485.h"


/************************************************
author			:ruanhugang
function		:RCC_Configuration
description		:System clock configuration
arg(input)		:null
return			:null
out(output)		:null
time			:2016.09.30
*************************************************/
void RCC_Configuration(void)
{
  /************************使能AHB时钟**************************/
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA , ENABLE);

  /************************使能APB2时钟**************************/
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_SYSCFG, ENABLE);

  /************************使能APB1时钟**************************/
}


/************************************************
author		:ruanhugang
function	:GPIO_Basic_Configuration
description	:GPIO configuration
arg(input)	:null
return		:null
out(output)	:null
time		:2016.09.30
*************************************************/
void GPIO_Basic_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  /************************ RS485接收端口配置 **********************/
  GPIO_PinAFConfig(GPIOA, RS485_RX_DATA0_PinSource9, 	GPIO_AF_1);
  GPIO_PinAFConfig(GPIOA, RS485_RX_DATA1_PinSource10, 	GPIO_AF_1);   
	
  GPIO_InitStructure.GPIO_Pin = RS485_RX_DATA0_Pin | RS485_RX_DATA1_Pin;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
  GPIO_InitStructure.GPIO_Pin = RS485_RXTX_CTRL1_Pin;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  /************************ RS485发送端口配置 **********************/
  GPIO_PinAFConfig(GPIOA, RS485_TX_DATA0_PinSource2, 	GPIO_AF_1);
  GPIO_PinAFConfig(GPIOA, RS485_TX_DATA0_PinSource3, 	GPIO_AF_1);   
	
  GPIO_InitStructure.GPIO_Pin = RS485_TX_DATA0_Pin | RS485_TX_DATA1_Pin;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
  GPIO_InitStructure.GPIO_Pin = RS485_RXTX_CTRL2_Pin;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
  /************************ WG输出端口配置 **********************/
  GPIO_InitStructure.GPIO_Pin = WG_DATA0_PIN | WG_DATA1_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
  /************************ WG输入端口配置 **********************/
  GPIO_InitStructure.GPIO_Pin = WG_EXTI_PIN_0|WG_EXTI_PIN_1;   
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	    
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;	 
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP	;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

}

/************************************************
author		:ruanhugang
function	:NVIC_Configuration
description	:NVIC configuration
arg(input)	:null
return		:null
out(output)	:null
time		:2016.09.30
*************************************************/
void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  /************************串口中断配置 **********************/
  NVIC_InitStructure.NVIC_IRQChannel 			= USART1_IRQn;               	/*IRQ通道:串口1*/
  NVIC_InitStructure.NVIC_IRQChannelPriority 	= 0;                    			/*优先级 :1级*/
  NVIC_InitStructure.NVIC_IRQChannelCmd 		= ENABLE;                   	/*使能IRQ通道*/
  NVIC_Init(&NVIC_InitStructure);
	
  /************************ 外部中断配置 **********************/
  NVIC_InitStructure.NVIC_IRQChannel 			= EXTI4_15_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority 	= 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd 		= ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}


/************************************************
author			:ruanhugang
function		:BASIC_Initializes
description	:Basic configuration
arg(input)	:null
return			:null
out(output)	:null
time				:2016.09.30
*************************************************/
void BASIC_Initializes(void)
{
  RCC_Configuration();                                               /*时钟配置*/
	
  GPIO_Basic_Configuration();                                        /*IO配置*/
	
  NVIC_Configuration();                                              /*NVIC配置*/
}

