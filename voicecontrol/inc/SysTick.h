#ifndef _SysTick_H
#define _SysTick_H

#include "system.h"

extern void Systick_Init(u8 SYSCLK);
extern void delay_ms(u16 nms);
extern void delay_us(u32 nus);



#endif
