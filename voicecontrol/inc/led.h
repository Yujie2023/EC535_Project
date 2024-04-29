#ifndef _led_H
#define _led_H
#include "system.h"



#define LED_KT PBout(12) //客厅灯引脚口
#define LED_WS PBout(0) //卧室灯引脚口
#define JDY_FS PBout(1) //控制继电器风扇引脚口
	
extern void ledshow(void);
extern void LED_Init(void);
#endif
