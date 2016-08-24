#ifndef __WIEGAND_H_
#define __WIEGAND_H_


//韦根协议选择 韦根26或韦根34
#define USE_WIEGAND26
//#define USE_WIEGAND34

//WIEGAND GPIO CONFIG
//DATA0 --> PA0      DATA1 -->PA1
#define RCC_WG_GPIO     RCC_AHBPeriph_GPIOA
#define WG_GPIO         GPIOA
#define WG_DATA0_PIN    GPIO_Pin_0
#define WG_DATA1_PIN    GPIO_Pin_1

//exit interrupt config
#define WG_PortSource       EXTI_PortSourceGPIOA
#define WG_DATA0_PinSource  EXTI_PinSource2
#define WG_DATA1_PinSource  EXTI_PinSource3

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

typedef struct sWiegandConfig
{
	void (*Init)(void);
	void (*ModeConfig)(unsigned char wg_mode);
	void (*SendData)(unsigned char *str);
	unsigned char (*ReceiveData)(unsigned long long src_wg34,unsigned char *dir_ptr);
}tWiegandConfig;


enum
{
	WG_RX_MODE,
	WG_TX_MODE,
	WG_ALL_MODE
};

 void WG_GPIO_Configuration(void);
 void WG_Send_Bit_1(void);
 void WG_Send_Bit_0(void);
 void WG_Send_Byte(unsigned char WG_Byte);
 void Send_Wiegand26(unsigned char *str);
 
 void WG_NVIC_Config(void);
 void WG_Config_RX(void);
 void WG_Config_TX(void);
 void WG_RTX_Mode(unsigned char wg_mode );
 unsigned char Get_WG26_Data(unsigned long long src_wg26 , unsigned char *dir_ptr);

	void Send_Wiegand34(unsigned char *str);
	unsigned char Get_WG34_Data(unsigned long long src_wg34 , unsigned char *dir_ptr);

	tWiegandConfig* WiegandInit(void);
 

void Soft_delay_us(unsigned short int time);
void Soft_delay_ms(unsigned short int time);
#endif
