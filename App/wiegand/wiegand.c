/******************************************************************************
   @file    wiegand.c 
   @author  ruanhugang
   @version V1.0
   @date    2015.09.30
   @brief   韦根数据的收发
******************************************************************************/
#include "wiegand.h"
#include "stm32f0xx.h"

tWiegandConfig WiegandConfig;

tWiegandConfig* WiegandInit(void)
{
#if defined(USE_WIEGAND26)
#elif defined(USE_WIEGAND34)
#elif defined(USE_WIEGAND42)
	WiegandConfig.SendData = Send_Wiegand ;
	WiegandConfig.ReceiveData = Get_WG26_Data ;
#else
  #error "Missing define: USE_WIEGANDxx"
#endif
	return &WiegandConfig;
}

void WG_Config_RX(void)
{
	
}


void WG_Config_TX(void)
{
}

void WG_RTX_Mode(uint8_t wg_mode )
{
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
    uint8_t *WG_Data;
    uint8_t even;       //韦根包前12位偶效验
    uint8_t odd;        //韦根包后12位齐效验
		
		
	  /*进行前12位的偶校验*/
	  even = ((str[0]&0x80)>>7)+((str[0]&0x40)>>6)+((str[0]&0x20)>>5)+((str[0]&0x10)>>4)+((str[0]&0x08)>>3)
	           +((str[0]&0x04)>>2)+((str[0]&0x02)>>1)+((str[0]&0x01))+((str[1]&0x80)>>7)+((str[1]&0x40)>>6)
						 +((str[1]&0x20)>>5)+((str[1]&0x10)>>4);
	
		/*进行后12位的奇校验*/
	  odd  = ((str[1]&0x08)>>3)+((str[1]&0x04)>>2)+((str[1]&0x02)>>1)+((str[1]&0x01))+((str[2]&0x80)>>7)
	          +((str[2]&0x40)>>6)+((str[2]&0x20)>>5)+((str[2]&0x10)>>4)+((str[2]&0x08)>>3)+((str[2]&0x04)>>2)
						+((str[2]&0x02)>>1)+(str[2]&0x01);
	  
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
			WG_Send_Byte(WG_Data[i]);
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


/************************************************

author			:	ruanhugang
function 		:	WG26_Check
description :	Validate WG data 
arg(input1)	:	src_wg26
return			:	1：校验成功；0：校验失败
out(output)	:	null
time				:	2016.09.30
*************************************************/
uint8_t WG26_Check(uint32_t src_wg26)
{
	uint8_t even_t,odd_t;
	uint8_t check_even , check_odd ,check_one;
//	uint8_t wg_t[3];
	
	
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
		/*可在此处加入自己的验证程序*/
		return 1;
	}
	else
		return 0;
}


/************************************************

author			:	ruanhugang
function 		:	Get_WG26_Data
description :	Convert int to uint8_t pointer
arg(input1)	:	src_wg26
return			:	1：获取数据成功；0：获取数据失败
out(output)	:	dir_ptr
time				:	2016.09.30
*************************************************/
uint8_t Get_WG26_Data(uint32_t src_wg26 , uint8_t *dir_ptr)
{
	if(WG26_Check(src_wg26))
	{
		*(dir_ptr++) =(uint8_t)src_wg26 >>17;   //MSB
		*(dir_ptr++) =(uint8_t)src_wg26 >> 9;
		*(dir_ptr)   =(uint8_t)src_wg26 >> 1;
		return 1;
	}else
	return 0;
}

/************************************************

author			:	ruanhugang
function 		:	Send_Wiegand34
description :	Send 34 bits WG Data
arg(input1)	:	str
return			:	null
out(output)	:	null
time				:2016.09.30
*************************************************/
void Send_Wiegand34(uint8_t *str)
{
    uint8_t i;
    uint8_t check_one;  //韦根包奇偶效验中间暂存
    uint8_t even;       //韦根包前20位偶效验
    uint8_t odd;        //韦根包后20位齐效验
 
	  /*进行前16位的偶校验*/
	  check_one = ((str[0]&0x80)>>7)+((str[0]&0x40)>>6)+((str[0]&0x20)>>5)+((str[0]&0x10)>>4)+((str[0]&0x08)>>3)+((str[0]&0x04)>>2)+((str[0]&0x02)>>1)+((str[0]&0x01))+
								((str[1]&0x80)>>7)+((str[1]&0x40)>>6)+((str[1]&0x20)>>5)+((str[1]&0x10)>>4)+((str[1]&0x08)>>3)+((str[1]&0x04)>>2)+((str[1]&0x02)>>1)+((str[1]&0x01))+
								((str[2]&0x80)>>7)+((str[2]&0x40)>>6)+((str[2]&0x20)>>5)+((str[2]&0x10)>>4);
	  if((check_one&0x01)== 0)
	    even = 0;			//偶校验 偶数个1
	  else  
		  even = 1;	    //偶校验 奇数个1
	  
	  /*进行后16位的奇校验*/
	  check_one = ((str[2]&0x08)>>3)+((str[2]&0x04)>>2)+((str[2]&0x02)>>1)+(str[2]&0x01)+
								((str[3]&0x80)>>7)+((str[3]&0x40)>>6)+((str[3]&0x20)>>5)+((str[3]&0x10)>>4)+((str[3]&0x08)>>3)+((str[3]&0x04)>>2)+((str[3]&0x02)>>1)+(str[3]&0x01)+
								((str[4]&0x80)>>7)+((str[4]&0x40)>>6)+((str[4]&0x20)>>5)+((str[4]&0x10)>>4)+((str[4]&0x08)>>3)+((str[4]&0x04)>>2)+((str[4]&0x02)>>1)+(str[4]&0x01);
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
	
    //发送40位数据
    for(i = 0;i<5;i++)
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


/************************************************

author			:	ruanhugang
function 		:	Send_Wiegand42
description :	Send 42 bits WG Data
arg(input)	:	str
return			:	null
out(output)	:	null
time				:2016.09.30
*************************************************/
void Send_Wiegand42(uint8_t *str)
{
    uint8_t i;
    uint8_t check_one;  //韦根包奇偶效验中间暂存
    uint8_t even;       //韦根包前20位偶效验
    uint8_t odd;        //韦根包后20位齐效验
 
	  /*进行前20位的偶校验*/
	  check_one = ((str[0]&0x80)>>7)+((str[0]&0x40)>>6)+((str[0]&0x20)>>5)+((str[0]&0x10)>>4)+((str[0]&0x08)>>3)+((str[0]&0x04)>>2)+((str[0]&0x02)>>1)+((str[0]&0x01))+
								((str[1]&0x80)>>7)+((str[1]&0x40)>>6)+((str[1]&0x20)>>5)+((str[1]&0x10)>>4)+((str[1]&0x08)>>3)+((str[1]&0x04)>>2)+((str[1]&0x02)>>1)+((str[1]&0x01))+
								((str[2]&0x80)>>7)+((str[2]&0x40)>>6)+((str[2]&0x20)>>5)+((str[2]&0x10)>>4);
	  if((check_one&0x01)== 0)
	    even = 0;			//偶校验 偶数个1
	  else  
		  even = 1;	    //偶校验 奇数个1
	  
	  /*进行后20位的奇校验*/
	  check_one = ((str[2]&0x08)>>3)+((str[2]&0x04)>>2)+((str[2]&0x02)>>1)+(str[2]&0x01)+
								((str[3]&0x80)>>7)+((str[3]&0x40)>>6)+((str[3]&0x20)>>5)+((str[3]&0x10)>>4)+((str[3]&0x08)>>3)+((str[3]&0x04)>>2)+((str[3]&0x02)>>1)+(str[3]&0x01)+
								((str[4]&0x80)>>7)+((str[4]&0x40)>>6)+((str[4]&0x20)>>5)+((str[4]&0x10)>>4)+((str[4]&0x08)>>3)+((str[4]&0x04)>>2)+((str[4]&0x02)>>1)+(str[4]&0x01);
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
	
    //发送40位数据
    for(i = 0;i<5;i++)
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


/************************************************

author			:	ruanhugang
function 		:	Send_Wiegand
description :	Send WG Data
arg(input1)	:	str
arg(input2)	:	type
return			:	null
out(output)	:	null
time				:	2016.09.30
*************************************************/
void Send_Wiegand(unsigned char *str,unsigned char type)
{
	if(type == WIEGAND42)
		Send_Wiegand42(str);
	else if(type == WIEGAND26)
		Send_Wiegand26(str);
}


void Soft_delay_us(uint16_t time)
{
	uint32_t i;  
   while(time--)
   {
      i=8;
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
      i=7950;
      while(i--) ;    
   }
}
