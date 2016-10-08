
#include "systick.h"

static __IO uint32_t TimingDelay;

uint8_t SYSTICK_Delay_Flag = 0;

/**
  * @brief  启动系统滴答定时器 SysTick
  * @param  无
  * @retval 无
  */
void SysTick_Init(void)
{
	//SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);	//选择外部时钟  HCLK/8
	//SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);	//选择外部时钟  HCLK
    
  /* SystemFrequency / 1000    1ms中断一次
	 * SystemFrequency / 100000	 10us中断一次
	 * SystemFrequency / 1000000 1us中断一次
	 */
	//if (SysTick_Config(SystemFrequency / 100000))	// ST3.0.0库版本
	if (SysTick_Config(SystemCoreClock / 100000))	// ST3.5.0库版本  使用内核时钟，不分频
	{
		while (1);
	}
	//设置滴答定时器优先级
	NVIC_SetPriority (SysTick_IRQn, 3 );   //优先级为 0：3
  //关闭滴答定时器  
  SysTick->CTRL &= ~ SysTick_CTRL_ENABLE_Msk;
	//开启滴答定时器
	//SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
}


/**
  * @brief	us延时程序,10us为一个单位
  * @param  
  *	@arg 		nTime: Delay_us( 1 ) 则实现的延时为 1 * 10us = 10us
  * @retval	无
  */
void Delay_us(__IO uint32_t nTime)
{
	SYSTICK_Delay_Flag = 1;
	
	TimingDelay = nTime;

  
	SysTick->VAL &= 0;//清空计数器 
	
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk ;//使能滴答定时器，使能滴答中断 
	
	while(TimingDelay != 0);
  
 
	SysTick->CTRL &= (~ SysTick_CTRL_TICKINT_Msk) & (~ SysTick_CTRL_ENABLE_Msk);//关闭滴答定时器，关闭滴答中断
  
	SysTick->VAL &= 0;//清空计数器
	
	SYSTICK_Delay_Flag = 0;
}

/**
  * @brief  获取节拍程序
  * @param  无
  * @retval 无
  * @attention  在 SysTick 中断函数 SysTick_Handler()调用
  */
void TimingDelay_Decrement(void)
{
	if (TimingDelay != 0x00)
	{ 
		TimingDelay--;
	}
}

