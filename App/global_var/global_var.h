
#ifndef __GLOBAL_VAR_H
#define __GLOBAL_VAR_H
#include "stm32f0xx.h"
#include "wiegand.h"



enum
{
	FREE = 0,
	OCCUPIED,
	DATA_COMING,
};

enum
{
	DEAL_485 = 1,
	DEAL_WG,
	BOTH
};

enum
{
	TIMEOUT = 1,
	TOO_SHORT
};

extern uint8_t  SYSTICK_Delay_Flag;
extern void TimingDelay_Decrement(void);
extern tWiegandConfig *WiegandUse;
/*Flag for EXTI*/
extern uint8_t FLAG_EXTI;

extern uint8_t FLAG_SENDER;

extern uint8_t FLAG_REPERTORY;

extern uint8_t FLAG_WG_CACHE;

extern uint8_t FLAG_485_CACHE;

/*wg data receive pointer*/
extern unsigned char WG_RCV_DATA[3];
/*485 data receive pointer*/
extern unsigned char RS485_RCV_DATA[12];
extern unsigned int WG_temp_buf ;
extern unsigned char RS485_Rcv_temp[13] ;



#endif
