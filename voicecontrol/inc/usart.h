#ifndef __USART_H
#define __USART_H
#include "system.h"
extern void USART1_Init(u32 bound);
extern void SendByte(u8 sedata);
extern void SendString(const u8 *str);
extern void SendArray(u8 data[],u16 len);
extern uint8 * itoa(int32 num);
extern uint8 * ftoa(float num, uint8 precision);

extern void PrintComBit(USART_TypeDef* USARTx, uint8_t Data);
extern void PrintCom(USART_TypeDef* USARTx, uint8_t *Data);
#endif


