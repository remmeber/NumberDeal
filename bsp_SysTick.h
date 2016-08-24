#ifndef __BSP_SYSTICK_H
#define __BSP_SYSTICK_H

#include "stm32f0xx.h"

void SysTick_Init(void);
void Delay_us(__IO uint32_t nTime);
#define Delay_ms(x) Delay_us(100*x)	 //单位ms

#endif /* __SYSTICK_H */
