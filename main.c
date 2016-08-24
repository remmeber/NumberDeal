/**
  ******************************************************************************
  * @file    Project/STM32F0xx_StdPeriph_Templates/main.c 
  * @author  MCD Application Team
  * @version V1.5.0
  * @date    05-December-2014
  * @brief   Main program body
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
#include "stm32f0xx.h"
#include "card_number.h"
#include "wiegand.h"
#include "global_var.h"
#include "bsp_SysTick.h"
#include "RS485.h"
#include <stdbool.h>
#include <stdio.h>


/** @addtogroup STM32F0xx_StdPeriph_Templates
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
void NVIC_Config(void);
void EXIT_Configuration(void);
void GPIO_Configuration(void);
void RCC_Configuration(void);

unsigned char *WG_SEND_DATA= NULL;
unsigned char *WG_RCV_DATA = NULL;
unsigned char *RS485_SEND_DATA = NULL;
unsigned char *RS485_RCV_DATA = NULL;

void Delay(__IO uint32_t nCount);

tWiegandConfig *WiegandUse = NULL; 
/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
	/*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f0xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f0xx.c file
     */
      
  /* Add your application code here
     */
 
	
	//滴答定时器初始化，在韦根接收的时候使用
	SysTick_Init();
	
	
	WiegandUse = WiegandInit();
	//wiegand config
	WiegandUse->Init();
	//wiegand rtx mode config 配置韦根收发模式
	WiegandUse->ModeConfig(WG_ALL_MODE);
	
  /* NVIC configuration */
  NVIC_Config();
	
	/* GPIO Configuration */
  GPIO_Configuration();
	
	//TX Init
	RS485_Init();
	
	//WiegandUse->SendData(temp_WG);
 	UART_485_Configuration();
		
	
	while(1){
		/*GPIO_SetBits(GPIOF,GPIO_Pin_9|GPIO_Pin_10);
		Delay(0x7FFFFF);
		GPIO_ResetBits(GPIOF,GPIO_Pin_9|GPIO_Pin_10);
		Delay(0x7FFFFF);*/
	}
}

/**
  * @brief  Configures the nested vectored interrupt controller.
  * @param  None
  * @retval None
  */
void NVIC_Config(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Enable the USART Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd =ENABLE;
  /* Enable the DMA Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel4_5_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 4;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief  Configures the different GPIO ports.
  * @param  None
  * @retval None
  */
void GPIO_Configuration(void)
{    
  GPIO_InitTypeDef GPIO_InitStructure;
  
  /* Enable GPIOA clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);     
  
  GPIO_StructInit(&GPIO_InitStructure);
  
  GPIO_DeInit(GPIOA);
  
  /* USART1 Pins configuration
    -PA09 = USART1_TX
    -PA10 = USART1_RX
    -PA12 = USART1_RTS (DE)
  */
	
    
  /* Connect pin to Periph*/
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_1);    
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_1);   
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_1); 
  
  /* 配置 USART1_Tx 作为推挽输出 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_Init(GPIOA, &GPIO_InitStructure);*/
  /* 配置 USART1_Rx 作为浮空输入 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_Init(GPIOA, &GPIO_InitStructure);*/
	
  /* Configure pins as AF pushpull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure);    
}

/**
  * @brief  Enables peripheral clocks.
  * @param  None
  * @retval None
  */
void RCC_Configuration(void)
{
  /* Enable USART1 clocks */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
  
  /* Configure SART1 clock */
  RCC_USARTCLKConfig(RCC_USART1CLK_SYSCLK);
  
  /* Enable DMA1 AHB clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
}


void Delay(__IO uint32_t nCount)
{
	while(nCount--){}
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
