#include "led.h"
#include "wiegand.h"

void Led_Init()
{
	
}

void GPIO_PinReverse(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
  /* Check the parameters */
  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
  assert_param(IS_GPIO_PIN(GPIO_Pin));
  
  GPIOx->ODR ^=  GPIO_Pin;
}

void LedBlink(uint8_t blink_times)
{
	uint8_t times = 0;
	LED_OFF;
	for(;times<blink_times;times++)
	{
		LED_ON;
		Soft_delay_ms(250);
		LED_OFF;
		Soft_delay_ms(250);
	}
}
