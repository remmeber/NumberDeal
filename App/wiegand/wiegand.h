#ifndef __WIEGAND_H_
#define __WIEGAND_H_


//韦根协议选择 韦根26或韦根34
//#define USE_WIEGAND26
//#define USE_WIEGAND34
#define USE_WIEGAND42

//WIEGAND GPIO CONFIG
//DATA0 --> PA0      DATA1 -->PA1
#define RCC_WG_GPIO     RCC_AHBPeriph_GPIOA
#define WG_GPIO         GPIOA
#define WG_DATA0_PIN    GPIO_Pin_0
#define WG_DATA1_PIN    GPIO_Pin_1
#define WG_EXTI_PIN_0		GPIO_Pin_4
#define WG_EXTI_PIN_1		GPIO_Pin_5//GPIO_Pin_15

//exit interrupt config
#define WG_PortSource       EXTI_PortSourceGPIOA
#define WG_DATA0_PinSource  EXTI_PinSource4
#define WG_DATA1_PinSource  EXTI_PinSource5//GPIO_Pin_15
#define WG_DATA0_EXIT_LINE	EXTI_Line4
#define WG_DATA1_EXIT_LINE	EXTI_Line5//GPIO_Pin_15

#define WG_DATA0_LOW    GPIO_ResetBits(WG_GPIO, WG_DATA0_PIN)
#define WG_DATA0_HIGH   GPIO_SetBits(WG_GPIO, WG_DATA0_PIN)
#define WG_DATA1_LOW    GPIO_ResetBits(WG_GPIO, WG_DATA1_PIN)
#define WG_DATA1_HIGH   GPIO_SetBits(WG_GPIO, WG_DATA1_PIN)




#define WG_MIN_D0_TIM  2
#define WG_MAX_D0_TIM  20

#define WG_MIN_BOUD_TIM  20
#define WG_MAX_BOUD_TIM  20000

#define WIEGAND26  25
#define WIEGAND34  33
#define WIEGAND42  41

typedef struct sWiegandConfig
{
	void (*SendData)(unsigned char *str,unsigned char type);
	unsigned char (*ReceiveData)(unsigned int src_wg34,unsigned char *dir_ptr);
}tWiegandConfig;

tWiegandConfig* WiegandInit(void);
 void WG_Send_Bit_1(void);
 void WG_Send_Bit_0(void);
 void WG_Send_Byte(unsigned char WG_Byte);
 void Send_Wiegand(unsigned char *str,unsigned char type);
 
 unsigned char WG26_Check(unsigned int src_wg26);

 unsigned char Get_WG26_Data(unsigned int src_wg26 , unsigned char *dir_ptr);


void Soft_delay_us(unsigned short int time);
void Soft_delay_ms(unsigned short int time);
#endif
