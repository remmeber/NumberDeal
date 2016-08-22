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
void UART_485_Configuration(void);
void NVIC_Config(void);
void EXIT_Configuration(void);

unsigned char *sendData = {0};
unsigned char *recvData = {0};

	void Delay(__IO uint32_t nCount);
void Delay(__IO uint32_t nCount)
{
while(nCount--){}
}

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
 
  UART_485_Configuration();
	
	WiegandUse = WiegandInit();
	//wiegand config
	WiegandUse->Init();
	//wiegand rtx mode config 配置韦根收发模式
	WiegandUse->ModeConfig(WG_ALL_MODE);
	
	//WiegandUse->SendData(temp_WG);
		
	
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
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

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
	USART_ClockInitTypeDef UART_ClockInitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
	
	/*串口时钟配置*/
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	/*串口时钟使能， GPIO 时钟使能*/
	USART_DeInit(USART1);
	
  /* 配置 USART1_Tx 作为推挽输出 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  /* 配置 USART1_Rx 作为浮空输入 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/*fill the structure data*/
	USART_StructInit(&UART_InitStructure);
	/* Configure USART1 basic and asynchronous paramters */
	USART_Init(USART1,&UART_InitStructure);
	
	USART_ClockStructInit(&UART_ClockInitStructure);
	/* Configure the USART1 synchronous paramters */
	USART_ClockInit(USART1,&UART_ClockInitStructure);
	
  /* NVIC configuration */
  NVIC_Config();
	
	/* Enable USART1 Receive interrupts */
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART1, ENABLE); //使能USART1	
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
