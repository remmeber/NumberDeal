/******************************************************************************
   @file    wiegand.c 
   @author  zhujj
   @version V1.0
   @date    2015.05.18
   @brief   韦根26数据发送
******************************************************************************/

#include "wiegand.h"
#include "stm32f0xx.h"

tWiegandConfig WiegandConfig;

tWiegandConfig* WiegandInit(void)
{
#if defined(USE_WIEGAND26)
	WiegandConfig.Init = WG_GPIO_Configuration ;
	WiegandConfig.ModeConfig = WG_RTX_Mode ;
	WiegandConfig.SendData = Send_Wiegand26 ;
	WiegandConfig.ReceiveData = Get_WG26_Data ;
#elif defined(USE_WIEGAND34)
	WiegandConfig.Init = WG_GPIO_Configuration ;
	WiegandConfig.ModeConfig = WG_RTX_Mode ;
	WiegandConfig.SendData = Send_Wiegand34 ;
	WiegandConfig.ReceiveData = Get_WG34_Data ;
#else
  #error "Missing define: USE_WIEGANDxx"
#endif
	return &WiegandConfig;
}

void WG_NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	/* 配置P[A|B|C|D|E]13为中断源 */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}


void WG_GPIO_Configuration(void)
{
  	GPIO_InitTypeDef GPIO_InitStructure;
    
    RCC_APB2PeriphClockCmd(RCC_WG_GPIO, ENABLE);   //GPIO时钟使能
    
    GPIO_InitStructure.GPIO_Pin = WG_DATA0_PIN | WG_DATA1_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	    //管脚速率选择
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;		//管脚功能配置
    GPIO_Init(WG_GPIO, &GPIO_InitStructure);				//端口选择：如PA，PB口
    
	GPIO_SetBits(WG_GPIO, GPIO_Pin_0);
	GPIO_SetBits(WG_GPIO, GPIO_Pin_1);
	
}

void WG_Config_RX(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	EXTI_InitTypeDef EXTI_InitStructure;

	/* config the extiline(PA0 PA1) clock and AFIO clock */
	RCC_APB2PeriphClockCmd(RCC_WG_GPIO,ENABLE);

	/* EXTI line gpio config(PA0 PA1) */	
	GPIO_InitStructure.GPIO_Pin = WG_DATA0_PIN | WG_DATA1_PIN;       
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;	 // 上拉输入
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(WG_GPIO, &GPIO_InitStructure);

	/* EXTI line(PA0) mode config */
	SYSCFG_EXTILineConfig(WG_PortSource, WG_DATA0_PinSource); 
	EXTI_InitStructure.EXTI_Line = EXTI_Line0;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //下降沿中断
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure); 
	
	/* EXTI line(PA1) mode config */
	SYSCFG_EXTILineConfig(WG_PortSource, WG_DATA1_PinSource); 
	EXTI_InitStructure.EXTI_Line = EXTI_Line1;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //下降沿中断
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
										
	/* config the NVIC(PA0 PA1) */
	WG_NVIC_Config();
}


void WG_Config_TX(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
    
    RCC_APB2PeriphClockCmd(RCC_WG_GPIO, ENABLE);   //GPIO时钟使能
    
    GPIO_InitStructure.GPIO_Pin = WG_DATA0_PIN | WG_DATA1_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	    //管脚速率选择
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;		//管脚功能配置
    GPIO_Init(WG_GPIO, &GPIO_InitStructure);				//端口选择：如PA，PB口
    
	GPIO_SetBits(WG_GPIO, GPIO_Pin_0);
	GPIO_SetBits(WG_GPIO, GPIO_Pin_1);
}

//wg_mode : WG_RX_MODE ,WG_TX_MODE
void WG_RTX_Mode(uint8_t wg_mode )
{
	if(wg_mode == WG_RX_MODE)
	{
		WG_Config_RX();
	}
	else if(wg_mode == WG_TX_MODE)
	{
		WG_Config_TX();
	}	else
	{
		WG_Config_RX();
		WG_Config_TX();
	}
}

//发送韦根数据1
void WG_Send_Bit_1(void)
{
	WG_DATA1_LOW;
    //延时100us
    Soft_delay_us(100);
	WG_DATA1_HIGH;
    //延时一个发送周期
    Soft_delay_ms(2);    
}

//发送韦根数据0
void WG_Send_Bit_0(void)
{
	WG_DATA0_LOW;
    //延时100us
    Soft_delay_us(100);
	WG_DATA0_HIGH;
    //延时一个发送周期
    Soft_delay_ms(2);    
}

//发送一个字节韦根数据
void WG_Send_Byte(uint8_t WG_Byte)
{
	uint8_t i;
	for(i=0;i<8;i++)
    {
		if(WG_Byte&0x80)
			WG_Send_Bit_1();
		else
			WG_Send_Bit_0();
		WG_Byte = WG_Byte << 1;
	}
}

//发送韦根26位数据
void Send_Wiegand26(uint8_t *str)
{
    uint8_t i;
    uint8_t check_one;  //韦根包奇偶效验中间暂存
    uint8_t even;       //韦根包前12位偶效验
    uint8_t odd;        //韦根包后12位齐效验
 
	  /*进行前12位的偶校验*/
	  check_one = ((str[0]&0x80)>>7)+((str[0]&0x40)>>6)+((str[0]&0x20)>>5)+((str[0]&0x10)>>4)+((str[0]&0x08)>>3)
	               +((str[0]&0x04)>>2)+((str[0]&0x02)>>1)+((str[0]&0x01))+((str[1]&0x80)>>7)+((str[1]&0x40)>>6)
								 +((str[1]&0x20)>>5)+((str[1]&0x10)>>4);
	  if((check_one&0x01)== 0)
	      even = 0;		//偶校验 偶数个1
	  else  
		  even = 1;	    //偶校验 奇数个1
	  
	  /*进行后12位的奇校验*/
	  check_one = ((str[1]&0x08)>>3)+((str[1]&0x04)>>2)+((str[1]&0x02)>>1)+((str[1]&0x01))+((str[2]&0x80)>>7)
	              +((str[2]&0x40)>>6)+((str[2]&0x20)>>5)+((str[2]&0x10)>>4)+((str[2]&0x08)>>3)+((str[2]&0x04)>>2)
								+((str[2]&0x02)>>1)+(str[2]&0x01);
	  if((check_one&0x01)== 1) 
		  odd = 0;    //奇校验 奇数个1
	  else  
		  odd = 1;    //奇校验 偶数个1
	  
    //韦根 输出端初始化
		WG_DATA0_HIGH;
		WG_DATA1_HIGH;
	
    //发送偶效验
    if(even == 0)
    {
        WG_Send_Bit_0();
    }
    else
    {
        WG_Send_Bit_1();
    }
	
    //发送24位数据
    for(i = 0;i<3;i++)
    {
		WG_Send_Byte(str[i]);
    }
	
    //发送奇效验位
    if(odd == 0)
    {
        WG_Send_Bit_0();
    }
    else
    {
        WG_Send_Bit_1();
    }
}


//韦根26数据转换成Byte数据
//返回 0 转换成功
//返回 1 转换失败
uint8_t Get_WG26_Data(uint64_t src_wg26 , uint8_t *dir_ptr)
{
	uint8_t even_t,odd_t;
	uint8_t check_even , check_odd ,check_one;
//	uint8_t wg_t[3];
	
	src_wg26 = (uint32_t)src_wg26;
	
	even_t = (src_wg26>>25) & 0x01;
	
	odd_t  = src_wg26 & 0x01;

	
	//奇偶校验
	check_one = ((src_wg26>>24)&1)+ ((src_wg26>>23)&1)+ ((src_wg26>>22)&1)+ ((src_wg26>>21)&1)+ ((src_wg26>>20)&1)+ ((src_wg26>>19)&1)+
		         ((src_wg26>>18)&1)+ ((src_wg26>>17)&1)+ ((src_wg26>>16)&1)+ ((src_wg26>>15)&1)+ ((src_wg26>>14)&1)+ ((src_wg26>>13)&1);
	if((check_one&0x01)== 0)
	    check_even = 0;		//偶校验 偶数个1
	else  
		check_even = 1;	    //偶校验 奇数个1
	
	check_one  = ((src_wg26>>12)&1)+ ((src_wg26>>11)&1)+ ((src_wg26>>10)&1)+ ((src_wg26>>9)&1)+ ((src_wg26>>8)&1)+ ((src_wg26>>7)&1)+ 
				 ((src_wg26>>6)&1)+ ((src_wg26>>5)&1)+ ((src_wg26>>4)&1)+ ((src_wg26>>3)&1)+ ((src_wg26>>2)&1)+ ((src_wg26>>1)&1);
	if((check_one&0x01)== 1)
		check_odd = 0;   //奇数个1
	else
		check_odd = 1;   //偶数个1
	
	if((even_t == check_even) && (odd_t == check_odd))
	{		
		*(dir_ptr++) = src_wg26 >>17;   //MSB
		*(dir_ptr++) = src_wg26 >>9;
		*(dir_ptr++) = src_wg26 >>1;
		
		//UART1_Send_char(WG_RX_buf,3); //测试收到数据
		//LED0_TOGGLE;
		
		return 0;
	}
	else
		return 1;
}




//发送韦根34位数据
void Send_Wiegand34(uint8_t *str)
{
    uint8_t i;
    uint8_t check_one;  //韦根包奇偶效验中间暂存
    uint8_t even;       //韦根包前12位偶效验
    uint8_t odd;        //韦根包后12位齐效验
 
	  /*进行前16位的偶校验*/
	  check_one = ((str[0]&0x80)>>7)+((str[0]&0x40)>>6)+((str[0]&0x20)>>5)+((str[0]&0x10)>>4)+((str[0]&0x08)>>3)+((str[0]&0x04)>>2)+((str[0]&0x02)>>1)+((str[0]&0x01))+
								((str[1]&0x80)>>7)+((str[1]&0x40)>>6)+((str[1]&0x20)>>5)+((str[1]&0x10)>>4)+((str[1]&0x08)>>3)+((str[1]&0x04)>>2)+((str[1]&0x02)>>1)+((str[1]&0x01));
	  if((check_one&0x01)== 0)
	    even = 0;			//偶校验 偶数个1
	  else  
		  even = 1;	    //偶校验 奇数个1
	  
	  /*进行后16位的奇校验*/
	  check_one = ((str[2]&0x80)>>7)+((str[2]&0x40)>>6)+((str[2]&0x20)>>5)+((str[2]&0x10)>>4)+((str[2]&0x08)>>3)+((str[2]&0x04)>>2)+((str[2]&0x02)>>1)+(str[2]&0x01)+
								((str[3]&0x80)>>7)+((str[3]&0x40)>>6)+((str[3]&0x20)>>5)+((str[3]&0x10)>>4)+((str[3]&0x08)>>3)+((str[3]&0x04)>>2)+((str[3]&0x02)>>1)+(str[3]&0x01);
	  if((check_one&0x01)== 1)
		  odd = 0;    //奇校验 奇数个1
	  else
		  odd = 1;    //奇校验 偶数个1
	  
    //韦根 输出端初始化
		WG_DATA0_HIGH;
		WG_DATA1_HIGH;
	
    //发送偶效验
    if(even == 0)
    {
        WG_Send_Bit_0();
    }
    else
    {
        WG_Send_Bit_1();
    }
	
    //发送32位数据
    for(i = 0;i<4;i++)
    {
			WG_Send_Byte(str[i]);
    }
	
    //发送奇效验位
    if(odd == 0)
    {
        WG_Send_Bit_0();
    }
    else
    {
        WG_Send_Bit_1();
    }
}

//韦根34数据转换成Byte数据
//返回 0 转换成功
//返回 1 转换失败
uint8_t Get_WG34_Data(uint64_t src_wg34 , uint8_t *dir_ptr)
{
	uint8_t even_t,odd_t;
	uint8_t check_even , check_odd ,check_one;
//	uint8_t wg_t[3];
	
	even_t = (src_wg34>>33) & 0x01;  //超过长度了
	
	odd_t  = src_wg34 & 0x01;

	
	//奇偶校验
	check_one = ((src_wg34>>32)&1)+ ((src_wg34>>31)&1)+ ((src_wg34>>30)&1)+ ((src_wg34>>29)&1)+ ((src_wg34>>28)&1)+ ((src_wg34>>27)&1)+((src_wg34>>26)&1)+ ((src_wg34>>25)&1)+
							((src_wg34>>24)&1)+ ((src_wg34>>23)&1)+ ((src_wg34>>22)&1)+ ((src_wg34>>21)&1)+((src_wg34>>20)&1)+ ((src_wg34>>19)&1)+ ((src_wg34>>18)&1)+ ((src_wg34>>17)&1);
	if((check_one&0x01)== 0)
	    check_even = 0;		//偶校验 偶数个1
	else  
		check_even = 1;	    //偶校验 奇数个1
	
	check_one = ((src_wg34>>16)&1)+ ((src_wg34>>15)&1)+ ((src_wg34>>14)&1)+ ((src_wg34>>13)&1)+ ((src_wg34>>12)&1)+ ((src_wg34>>11)&1)+((src_wg34>>10)&1)+ ((src_wg34>>9)&1)+
							((src_wg34>>8)&1)+ ((src_wg34>>7)&1)+ ((src_wg34>>6)&1)+ ((src_wg34>>5)&1)+((src_wg34>>4)&1)+ ((src_wg34>>3)&1)+ ((src_wg34>>2)&1)+ ((src_wg34>>1)&1);
	if((check_one&0x01)== 1)
		check_odd = 0;   //奇数个1
	else
		check_odd = 1;   //偶数个1
	
	if((even_t == check_even) && (odd_t == check_odd))
	{		
		*(dir_ptr++) = src_wg34 >>25;  //MSB
		*(dir_ptr++) = src_wg34 >>17;   
		*(dir_ptr++) = src_wg34 >>9 ;
		*(dir_ptr++) = src_wg34 >>1 ;
		
		//UART1_Send_char(WG_RX_buf,4);  //测试收到数据
		//LED0_TOGGLE;
		
		return 0;
	}
	else
		return 1;
}


void Soft_delay_us(uint16_t time)
{
	uint32_t i;  
   while(time--)
   {
      i=8;  //ؔܺ֨ӥ  ַ˔ʱݤú20141116
      while(i--){};
        __nop();
        __nop();
        __nop();
        __nop();
   }
}
void Soft_delay_ms(uint16_t time)
{
	  uint16_t i=0;  
   while(time--)
   {
      i=7950;  //ؔܺ֨ӥ  ַ˔ʱݤú20141116
      while(i--) ;    
   }
}
