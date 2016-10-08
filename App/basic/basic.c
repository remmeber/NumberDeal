
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
  /************************ʹ��AHBʱ��**************************/
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA , ENABLE);

  /************************ʹ��APB2ʱ��**************************/
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_SYSCFG, ENABLE);

  /************************ʹ��APB1ʱ��**************************/
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
  /************************ RS485���ն˿����� **********************/
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
  
  /************************ RS485���Ͷ˿����� **********************/
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
	
  /************************ WG����˿����� **********************/
  GPIO_InitStructure.GPIO_Pin = WG_DATA0_PIN | WG_DATA1_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
  /************************ WG����˿����� **********************/
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

  /************************�����ж����� **********************/
  NVIC_InitStructure.NVIC_IRQChannel 			= USART1_IRQn;               	/*IRQͨ��:����1*/
  NVIC_InitStructure.NVIC_IRQChannelPriority 	= 0;                    			/*���ȼ� :1��*/
  NVIC_InitStructure.NVIC_IRQChannelCmd 		= ENABLE;                   	/*ʹ��IRQͨ��*/
  NVIC_Init(&NVIC_InitStructure);
	
  /************************ �ⲿ�ж����� **********************/
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
  RCC_Configuration();                                               /*ʱ������*/
	
  GPIO_Basic_Configuration();                                        /*IO����*/
	
  NVIC_Configuration();                                              /*NVIC����*/
}

