
#ifndef __GLOBAL_VAR_H
#define __GLOBAL_VAR_H
#include "stm32f0xx.h"
#include "wiegand.h"

extern uint8_t  SYSTICK_Delay_Flag;
extern void TimingDelay_Decrement(void);
extern tWiegandConfig *WiegandUse;
/*wg data send pointer*/
extern unsigned char *WG_SEND_DATA;
/*wg data receive pointer*/
extern unsigned char *WG_RCV_DATA;
/*485 data send pointer*/
extern unsigned char *RS485_SEND_DATA;
/*485 data receive pointer*/
extern unsigned char *RS485_RCV_DATA;

#endif
