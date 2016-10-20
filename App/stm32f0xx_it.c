/**
  ******************************************************************************
  * @file    GPIO/GPIO_IOToggle/stm32f0xx_it.c 
  * @author  MCD Application Team
  * @version V1.4.0
  * @date    24-July-2014
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_it.h"
#include "stm32f0xx_usart.h"
#include "global_var.h"
#include "RS485.h"

volatile uint16_t tim_cnt = 0;
uint16_t tim_free_cnt = 0;//??????????
uint8_t FLAG_CHECK_FREE = 0;

uint8_t  WG_cnt = 0;
uint8_t count = 0;
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M0 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
		if(FLAG_CHECK_FREE)
			tim_free_cnt++;
		if(tim_free_cnt>=WG_MAX_BOUD_TIM){
			SysTick->CTRL &= ~ SysTick_CTRL_ENABLE_Msk;  //???????
			FLAG_EXTI |= TIMEOUT;
			if(WG_cnt<=WIEGAND26)
				FLAG_EXTI |= TOO_SHORT;//设置WG格式错误标志位
			WG_cnt = 0;
			WG_temp_buf = 0;
			tim_cnt = 0;
			FLAG_CHECK_FREE = 0;
			return;
		}
		if(SYSTICK_Delay_Flag)
		{
			tim_cnt++;
		}
		//TimingDelay_Decrement();
}


/************************************************

author			:	ruanhugang
function 		:	USART1_IRQHandler
description :	usart interrupt service function
arg(input)	:	null
return			:	null
out(output)	:	null
time				:2016.09.30
*************************************************/
void USART1_IRQHandler(void)
{
	uint8_t temp = 0;
	//if (USART_GetFlagStatus(USART1, USART_FLAG_ORE) != RESET)//??!????if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)???T
	if(USART_GetFlagStatus(USART1,USART_FLAG_ORE)!=RESET)
	{
		USART_ReceiveData(USART1);
		USART_ClearITPendingBit(USART1, USART_IT_ORE);
	}
	if(USART_GetITStatus(USART1,USART_IT_RXNE)!=RESET){
		
		if(FLAG_485_CACHE == OCCUPIED)
			return;
		temp = USART_ReceiveData(USART1)&0xff;
		if(FLAG_DATA == FREE && temp == 'B' )
			FLAG_DATA = DATA_COMING;
		RS485_Rcv_temp[count] = temp;
		count++;
		if(count==12)//
		{
			count=0;
			if((FLAG_REPERTORY & DEAL_485) == FREE){
				RS485_Data_Rcv(RS485_Rcv_temp,RS485_RCV_DATA);
				FLAG_485_CACHE = FREE;
				FLAG_DATA = FREE;
				FLAG_REPERTORY |= DEAL_485;
			}
			else FLAG_485_CACHE = OCCUPIED;
			//USART_Printf(RS485_RCV_DATA);
			USART_ClearITPendingBit(USART1, USART_IT_RXNE);
			
	}
  }
}

/************************************************

author			:	ruanhugang
function 		:	EXTI4_15_IRQHandler
description :	External interrupt service function
arg(input)	:	null
return			:	null
out(output)	:	null
time				:2016.09.30
*************************************************/

void EXTI4_15_IRQHandler(void)
{
	if(FLAG_WG_CACHE == OCCUPIED){
			return;
	}
  if(EXTI_GetITStatus(WG_DATA0_EXIT_LINE) != RESET) //???????EXTI Line??
	{
		FLAG_EXTI = RESET;//清除外部中断超时标志位，只要重新中断，就把中断检测标志位清0，进入下一间隔的检测
		
		if(tim_free_cnt>0)//???????????,?????????
			tim_free_cnt=0;
		if(!FLAG_CHECK_FREE)
			FLAG_CHECK_FREE = 1;//??????
		if((GPIO_ReadInputDataBit(WG_GPIO,WG_EXTI_PIN_1) == 1))// && (tim_cnt <= WG_MAX_BOUD_TIM))  //???????bit??????
		{
			//SYSTICK_Delay_Flag = 1;
			//SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;  //???????  //10us????
			SysTick->VAL = 0;
			while(GPIO_ReadInputDataBit(WG_GPIO,WG_EXTI_PIN_0) == 0)  //????
			{
				SYSTICK_Delay_Flag = 1;
				SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;  //???????  //10us????
				//SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;  //???????  //10us????
			}
			SYSTICK_Delay_Flag = 0;
			
			//SysTick->CTRL &= ~ SysTick_CTRL_ENABLE_Msk;  //???????

			if((GPIO_ReadInputDataBit(WG_GPIO,WG_EXTI_PIN_1) == 1) && (tim_cnt >= WG_MIN_D0_TIM) && (tim_cnt <= WG_MAX_D0_TIM))  //????
			{
				WG_temp_buf = WG_temp_buf << 1;
				WG_temp_buf = WG_temp_buf + 0;
				WG_cnt ++;
				tim_cnt = 0;
			}
			else
			{
				WG_temp_buf = 0; 
				WG_cnt = 0;
				tim_cnt = 0;
			}
			
			
		}
		EXTI_ClearITPendingBit(WG_DATA0_EXIT_LINE);     //???????
	}else	if(EXTI_GetITStatus(WG_DATA1_EXIT_LINE) != RESET)
	{
		
		if(!FLAG_CHECK_FREE)
			FLAG_CHECK_FREE = 1;//打开中断空闲检测
		
		if((GPIO_ReadInputDataBit(WG_GPIO,WG_EXTI_PIN_0) == 1))// && (tim_cnt <= WG_MAX_BOUD_TIM))  
		{
			//SYSTICK_Delay_Flag = 1;
			//SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;  //打开滴答定时器  //10us一周期
			SysTick->VAL = 0;
			while(GPIO_ReadInputDataBit(WG_GPIO,WG_EXTI_PIN_1) == 0)  //读取端口数据
			{
				SYSTICK_Delay_Flag = 1;
				SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;  //詹擢謳瞻吱时欠  //10us一诇菤
			}
			//SysTick->CTRL &= ~ SysTick_CTRL_ENABLE_Msk;
			SYSTICK_Delay_Flag = 0;
			
			if((GPIO_ReadInputDataBit(WG_GPIO,WG_EXTI_PIN_0) == 1) && (tim_cnt >= WG_MIN_D0_TIM) && (tim_cnt <= WG_MAX_D0_TIM))  //????
			{
				WG_temp_buf = WG_temp_buf << 1;
				WG_temp_buf = WG_temp_buf + 1;
				WG_cnt ++;
				tim_cnt = 0;
			}
			else
			{
				WG_temp_buf = 0; 
				WG_cnt = 0;
				tim_cnt = 0;
			}			
		}		
		EXTI_ClearITPendingBit(WG_DATA1_EXIT_LINE);     //清除中断标志
	}
	
	if(WG_cnt > WIEGAND26)
	{
		SysTick->CTRL &= ~ SysTick_CTRL_ENABLE_Msk;  //关闭滴答定时器
		if((FLAG_REPERTORY & DEAL_WG) == FREE && WiegandUse->ReceiveData(WG_temp_buf,WG_RCV_DATA))
		{
			FLAG_WG_CACHE = FREE;
			FLAG_REPERTORY |= DEAL_WG;
		}else FLAG_WG_CACHE = OCCUPIED;
		WG_temp_buf = 0;
		WG_cnt = 0;
		tim_cnt = 0;
	}
}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
