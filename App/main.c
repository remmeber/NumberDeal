#include "basic.h"
#include "stm32f0xx_usart.h"
#include "vehicle_number.h"
#include "wiegand.h"
#include "global_var.h"
#include "systick.h"
#include "RS485.h"
#include <stdbool.h>
#include <stdio.h>

/******************WG数据接收数组*****************/
unsigned char WG_RCV_DATA[3] = {0};

/*****************485数据接收数组*****************/
unsigned char RS485_RCV_DATA[12] = {0};

/******************WG数据临时存储*****************/
uint32_t WG_temp_buf = 0;

/*****************485数据临时存储*****************/
uint8_t RS485_Rcv_temp[13] = {0};

/*******************中断标志位********************/
uint8_t FLAG_EXTI = FREE;

/*******************处理标志位********************
uint8_t FLAG_DEAL = FREE;
**************************************************/
/*******************发送标志位********************
uint8_t FLAG_SENDER = FREE;
**************************************************/

/******************发送仓库标志位*****************/
uint8_t FLAG_REPERTORY = FREE;
/********************WG缓存标志位*****************/
uint8_t FLAG_WG_CACHE = FREE;
/******************485缓存标志位******************/
uint8_t FLAG_485_CACHE = FREE;
/*********************WG配置**********************/
tWiegandConfig *WiegandUse = NULL; 

void EXIT_Configuration(void);
void USART_Initializes(void);
void GPIO_Configuration(void);
uint8_t Deal_485_Data(uint8_t *input,uint8_t *result);
uint8_t Deal_WG_Data(uint8_t *WG_Data);

/************************************************
author			ruanhugang
function 		System_Initializes
description 	Initialize the system configuration
arg(input)		null
return			null
out(outpur)		null
time			2016.09.30
*************************************************/
void System_Initializes(void)
{
	BASIC_Initializes();
	
	USART_Initializes();
	
	EXIT_Configuration();
	
	SysTick_Init();
	
	WiegandUse = WiegandInit();
}

/************************************************

author			ruanhugang
function		main
description		The enrance of the main function
arg				null
return			int
out				null
time			2016.09.30
*************************************************/
int main(void)
{
	uint8_t result[5] = {0};
	
	//unsigned char *test = getVehicle_Number_List();
	//RS485_Data_Rcv(test,RS485_RCV_DATA);
	//FLAG_REPERTORY |= DEAL_485;
	
	System_Initializes();
	
	USART_Printf((uint8_t*)"usart debug\r\n");		//供测试使用
	
	while(1)
  {
		switch(FLAG_REPERTORY)
		{
			case FREE:
				//Soft_delay_ms(10);											//空闲状态加入延时，防止mcu长期空闲运行
				break;
			case DEAL_485:
					Deal_485_Data(RS485_RCV_DATA,result);
					Soft_delay_ms(10);
				//FLAG_REPERTORY |= DEAL_485;
				break;
			case DEAL_WG:
				Deal_WG_Data(WG_RCV_DATA);
				//FLAG_REPERTORY |= DEAL_WG;
				USART_Printf((uint8_t*)"WG debug\r\n");
					Soft_delay_ms(10);
				break;
			case BOTH:
				if(Deal_485_Data(RS485_RCV_DATA,result))
				{
					Deal_WG_Data(WG_RCV_DATA);
				}
				break;
		}
		
		
		if((FLAG_EXTI & TIMEOUT) == TIMEOUT)
		{
			USART_Printf((uint8_t*)"external timeout");
			FLAG_EXTI &= ~TIMEOUT;
		}
		if((FLAG_EXTI & TOO_SHORT) == TOO_SHORT)
		{
			USART_Printf((uint8_t*)"WG length too short");
			FLAG_EXTI &= ~TOO_SHORT;
		}
  }
}

/************************************************

author			ruanhugang
function 		Deal_WG_Data
description	To deal WG data, and send it
arg					WG_Data
return			uint8_t
out					null
time				2016.09.30
*************************************************/
uint8_t Deal_WG_Data(uint8_t *WG_Data)
{
	uint32_t temp_WG = 0;
	uint8_t i = 0;
	if(WG_Data == NULL)
		return 0 ;
	
		FLAG_REPERTORY &= ~DEAL_WG;
		if(FLAG_WG_CACHE == OCCUPIED)			//处理完当前韦根后，立即查询缓存标志位，提高处理效率
		{
			WiegandUse->ReceiveData(WG_temp_buf,WG_RCV_DATA);
			FLAG_WG_CACHE = FREE;
			FLAG_REPERTORY |= DEAL_WG;
		}
		if((FLAG_REPERTORY & DEAL_485) == FREE && FLAG_485_CACHE == OCCUPIED)
		{																	
			FLAG_485_CACHE = FREE;
			FLAG_REPERTORY |= DEAL_485;
		}
		WiegandUse->SendData(WG_Data,WIEGAND26);
		FLAG_WG_CACHE = FREE;
}

/************************************************

author				ruanhugang
function 			dealCarNumber
description 	To deal received car number, and output it with deal state
arg(input)		input
return				uint8_t (success 1,error 0)
out(output)		out
time					2016.09.30
*************************************************/

uint8_t dealCarNumber(uint8_t *input,uint8_t *out)
{
	uint8_t position=0;
	uint16_t firstAsciiOut = 0;
	uint32_t secondAsciiOut = 0;
	//uint8_t outNum = 0;
	if(input == NULL||out == NULL)
		return 0;
	if(RS485_Check(input))
	{
		input++;
		firstAsciiOut = firstPartNumberOut(input,&position);
		secondAsciiOut = secondPartNumberOut(&input[position]);
		if(assembleAscii(&firstAsciiOut,&secondAsciiOut,2,out))
		{
			return 1;
		}
	}
	return 0;
}

/************************************************

author			ruanhugang
function 		Deal_485_Data
description	To deal received 485 data, and output it with deal state
arg(input)	input
return			uint8_t (success 1,error 0)
out(output)	out
time				2016.09.30
*************************************************/
uint8_t Deal_485_Data(uint8_t *input,uint8_t *result)
{
	if(dealCarNumber(input,result))
	{
		FLAG_REPERTORY &= ~DEAL_485;
		if(FLAG_485_CACHE == OCCUPIED)		//处理完485数据后，立即去查询485缓存标志位，提高处理效率
		{
			RS485_Data_Rcv(RS485_Rcv_temp,RS485_RCV_DATA);
			FLAG_485_CACHE = FREE;
			FLAG_REPERTORY |= DEAL_485;
		}
		if((FLAG_REPERTORY & DEAL_WG) == FREE && FLAG_WG_CACHE == OCCUPIED && WiegandUse->ReceiveData(WG_temp_buf,WG_RCV_DATA))
		{																	
			FLAG_WG_CACHE = FREE;
			FLAG_REPERTORY |= DEAL_WG;
		}
			//USART_Printf(result);
		WiegandUse->SendData(result,WIEGAND42);
			//Send_Wiegand34(result);
		return 1;
	}
	else
	{ 
		/**/
		return 0;
	}
}

/************************************************

author			ruanhugang
function 		EXIT_Configuration
description	external interrupt configuration
arg(input)	null
return			null
out(output)	null
time				2016.09.30
*************************************************/
void EXIT_Configuration(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	/***************外部中断线0设置*****************/
	EXTI_InitStructure.EXTI_Line = WG_DATA0_EXIT_LINE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //下降沿
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure); 
	SYSCFG_EXTILineConfig(WG_PortSource, WG_DATA0_PinSource); 
	
	/***************外部中断线1设置******************/
	EXTI_InitStructure.EXTI_Line = WG_DATA1_EXIT_LINE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; 
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	SYSCFG_EXTILineConfig(WG_PortSource, WG_DATA1_PinSource); 
}

