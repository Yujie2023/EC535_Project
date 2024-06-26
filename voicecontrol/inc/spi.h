#ifndef __SPI_H
#define __SPI_H
#include "system.h"

							  
void SPI2_Init(void);			 //初始化SPI口
void SPI2_SetSpeed(u8 SpeedSet); //设置SPI速度   
u8 SPI2_ReadWriteByte(u8 TxData);//SPI总线读写一个字节


void SPI1_Init(void);			 //初始化SPI口
void SPI1_SetSpeed(uint8_t speed); //设置SPI速度   
uint8_t SPI1_ReadWriteByte(uint8_t dat);//SPI总线读写一个字节

		 
#endif

