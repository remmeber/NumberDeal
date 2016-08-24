/**
  ******************************************************************************
  * @file    Project/STM32F0xx_StdPeriph_Templates/stm32f0xx_it.c 
  * @author  MCD Application Team
  * @version V1.5.0
  * @date    05-December-2014
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
#include "stm32f0xx.h"
#include "global_var.h"
#include "wiegand.h"
#include "bsp_SysTick.h"

/** @addtogroup Template_Project
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/


volatile uint16_t tim_cnt = 0;

uint64_t WG_temp_buf = 0;
uint8_t  WG_cnt = 0;
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
		tim_cnt++;
		if(SYSTICK_Delay_Flag)
		{
			TimingDelay_Decrement();
		}
}
/**
  * @brief  This function handles DMA1 Channel2 interrupt request.
  * @param: None
  * @retval: None
  */

void DMA1_Channel4_5_IRQHandler(void)
{
  if(DMA_GetFlagStatus(DMA1_FLAG_TC4)==SET)
  {
    /* Enable USART1 Transmit complete interrupt */
    USART_ITConfig(USART1, USART_IT_TC, ENABLE); 
    
    /* Disable DMA1 Channel4 transmit complete interrupt */
    DMA_ITConfig(DMA1_Channel4, DMA_IT_TC, DISABLE);
  
    /* Clear DMA TC pending bit */
    DMA_ClearITPendingBit(DMA1_IT_TC4);
  }
  
  if(DMA_GetFlagStatus(DMA1_FLAG_TC5)==SET)
  {
    /* Enable USART1 Transmit complete interrupt */
    USART_ITConfig(USART1, USART_IT_TC, ENABLE); 
    
    /* Disable DMA1 Channel5 transmit complete interrupt */
    DMA_ITConfig(DMA1_Channel5, DMA_IT_TC, DISABLE);
  
    /* Clear DMA TC pending bit */
    DMA_ClearITPendingBit(DMA1_IT_TC5);
  }
}

/*
 *串口1中断服务函数
 */
void USART1_IRQHandler(void)
{
  while(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
  {
    /* Read one byte from the receive data register */
    //RxBuffer[RxCount++] = (USART_ReceiveData(USART1) & 0x7F);

    //if(RxCount == NbrOfDataToRead)
    //{
      /* Disable the EVAL_COM1 Receive interrupt */
		*(RS485_RCV_DATA++) = (USART_ReceiveData(USART1) & 0xFF);
    //}
  }
  USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);

  if(USART_GetITStatus(USART1, USART_IT_TXE) != RESET)
  {   
    /* Write one byte to the transmit data register */
    //USART_SendData(USART1, TxBuffer[TxCount++]);

    //if(TxCount == NbrOfDataToTransfer) 
    //{
      /* Disable the EVAL_COM1 Transmit interrupt */
      USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
    //}
  }
}

void EXTI0_1_IRQHandler(void)
{
  	if(EXTI_GetITStatus(EXTI_Line0) != RESET) //确保是否产生了EXTI Line中断
	{
		if((GPIO_ReadInputDataBit(WG_GPIO,WG_DATA1_PIN) == 1))// && (tim_cnt <= WG_MAX_BOUD_TIM))  //判断等待下一个bit数据是否超时
		{
			while(GPIO_ReadInputDataBit(WG_GPIO,WG_DATA0_PIN) == 0)  //计算脉宽
			{
				SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;  //开启滴答定时器  //10us定时中断
			}
			SysTick->CTRL &= ~ SysTick_CTRL_ENABLE_Msk;  //关闭滴答定时器

			if((GPIO_ReadInputDataBit(WG_GPIO,WG_DATA1_PIN) == 1) && (tim_cnt >= WG_MIN_D0_TIM) && (tim_cnt <= WG_MAX_D0_TIM))  //判断脉宽
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
			
#if defined (USE_WIEGAND26)
			if(WG_cnt > WIEGAND26)    //读出一帧韦根数据
#elif defined (USE_WIEGAND34)
			if(WG_cnt > WIEGAND34)    //读出一帧韦根数据
#else
    #error "Missing define: USE_WIEGANDxx"
#endif
			{
				WiegandUse->ReceiveData(WG_temp_buf,WG_RCV_DATA);
				WG_temp_buf = 0;
				WG_cnt = 0;
				tim_cnt = 0;
			}
			
		}
		EXTI_ClearITPendingBit(EXTI_Line0);     //清除中断标志位
	}  
}

void EXTI2_3_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line2) != RESET) //确保是否产生了EXTI Line中断
	{
    if((GPIO_ReadInputDataBit(WG_GPIO,WG_DATA0_PIN) == 1))// && (tim_cnt <= WG_MAX_BOUD_TIM))  
		{
			while(GPIO_ReadInputDataBit(WG_GPIO,WG_DATA1_PIN) == 0)  //计算脉宽
			{
				SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
			}
			SysTick->CTRL &= ~ SysTick_CTRL_ENABLE_Msk;
			
			if((GPIO_ReadInputDataBit(WG_GPIO,WG_DATA0_PIN) == 1) && (tim_cnt >= WG_MIN_D0_TIM) && (tim_cnt <= WG_MAX_D0_TIM))  //判断脉宽
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
			
#if defined (USE_WIEGAND26)
			if(WG_cnt > WIEGAND26)    //读出一帧韦根数据
#elif defined (USE_WIEGAND34)
			if(WG_cnt > WIEGAND34)    //读出一帧韦根数据
#else
    #error "Missing define: USE_WIEGANDxx"
#endif
			{
				WiegandUse->ReceiveData(WG_temp_buf,WG_RCV_DATA);
				WG_temp_buf = 0;
				WG_cnt = 0;
				tim_cnt = 0;
			}
			
		}
		EXTI_ClearITPendingBit(EXTI_Line1);     //清除中断标志位
	}  
}

/******************************************************************************/
/*                 STM32F0xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f0xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
