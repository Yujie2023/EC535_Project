#ifndef _ds18b20_H
#define _ds18b20_H

#include "system.h"

#define DS0 GPIO_Pin_0	 	//管脚宏定义


////IO操作函数											   
#define	DS18B20_0_DQ_OUT1 GPIO_SetBits(GPIOC,DS0)	 //IO口输出高电平
#define	DS18B20_0_DQ_OUT0 GPIO_ResetBits(GPIOC,DS0) //IO口输出低电平
#define DS18B20_0_DQ_IN   GPIO_ReadInputDataBit(GPIOC,DS0) //获取的状态



extern float DsTemp[6];

void DS18B20_0_IO_IN(void);
void DS18B20_0_IO_OUT(void);
uint8_t DS18B20_0_Init(void);			//初始化DS18B20
float DS18B20_0_GetTemperture(void);	//获取温度
void DS18B20_0_Start(void);		//开始温度转换
void DS18B20_0_Write_Byte(uint8_t dat);//写入一个字节
uint8_t DS18B20_0_Read_Byte(void);		//读出一个字节
uint8_t DS18B20_0_Read_Bit(void);		//读出一个位
uint8_t DS18B20_0_Check(void);			//检测是否存在DS18B20
void DS18B20_0_Reset(void);			//复位DS18B20  


void Ds18b20Init(void);
int get18b20(void);


#endif
