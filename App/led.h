#ifndef __LED_H_
#define __LED_H_
#include <stdint.h>
#include "stm32f0xx.h"

#define LED_PIN GPIO_Pin_12
#define LED_ON  GPIO_ResetBits(GPIOA, LED_PIN)
#define LED_OFF  GPIO_SetBits(GPIOA, LED_PIN)
#define LED_REVERSE GPIO_PinReverse(GPIOA,LED_PIN)

void LedBlink(uint8_t blink_times);
void GPIO_PinReverse(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
#endif

