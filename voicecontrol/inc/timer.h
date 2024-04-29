#ifndef _TIMER_H
#define _TIMER_H
#include "variable.h"

#define TIME_1MS		1
#define TIME_5MS		5
#define TIME_10MS		10
#define TIME_20MS		20
#define TIME_50MS		50
#define TIME_100MS	100
#define TIME_200MS	200
#define TIME_500MS	500

struct Time_Tick_Flag{
	UINT16 bTimer1ms 		: 1;
	UINT16 bTimer5ms 		: 1;
	UINT16 bTimer10ms 	: 1;
	UINT16 bTimer20ms 	: 1;
	UINT16 bTimer50ms 	: 1;
	UINT16 bTimer100ms 	: 1;
	UINT16 bTimer200ms 	: 1;
	UINT16 bTimer500ms 	: 1;
	UINT16 bRvd 				: 8;
};

extern volatile struct Time_Tick_Flag g_stTimerTickFlag;
extern UINT16 g_u16BaseTimer;


extern void TIM3_Int_Init(u16 arr,u16 psc);

extern void TIM4_Int_Init(u16 arr,u16 psc);
extern void TimerTreat(void);

extern void PWM_GPIO_Config(void);
extern void PWM_Mode_Config(void);
extern void PWM_Init(void);
extern void PWM1_Duty(uint16_t val);
extern void PWM2_Duty(uint16_t val);
extern void PWM3_Duty(uint16_t val);
extern void PWM4_Duty(uint16_t val);
#endif
