
#include "LDchip.h"
#include "Reg_RW.h"
#include "system.h"
#include "SysTick.h"
#include "usart.h"
#include <stdio.h>
#include "led.h"
/*************端口信息********************
 * 接线说明
 LD3320接口 	  STM32接口
 * RST              PB6
 * CS   	        PB8
 * WR/SPIS          PB9
 * P2/SDCK          PB13
 * P1/SDO           PB14
 * P0/SDI           PB15
 * IRQ              PA3
 * A0		        PB7
 * RD               PA0

*****************************************/



/************************************************************************************
//	nAsrStatus 用来在main主程序中表示程序运行的状态，不是LD3320芯片内部的状态寄存器
//	LD_ASR_NONE:		表示没有在作ASR识别
//	LD_ASR_RUNING：		表示LD3320正在作ASR识别中
//	LD_ASR_FOUNDOK:		表示一次识别流程结束后，有一个识别结果
//	LD_ASR_FOUNDZERO:	表示一次识别流程结束后，没有识别结果
//	LD_ASR_ERROR:		表示一次识别流程中LD3320芯片内部出现不正确的状态
*********************************************************************************/

uint8 nAsrStatus=0;	
void LD3320_Init(void);
uint8 RunASR(void);
void ProcessInt0(void);
void LD3320_EXTI_Cfg(void);
void LD3320_Spi_cfg(void);
void LD3320_GPIO_Cfg(void);
void LED_gpio_cfg(void);
static void Board_text(uint8 Code_Val);
extern void PrintComBit(USART_TypeDef* USARTx, uint8_t Data);
/***********************************************************
* 名    称： LD3320_main(void)
* 功    能： 主函数LD3320程序入口
* 入口参数：  
* 出口参数：
* 说    明：
* 调用方法： 
**********************************************************/ 
void  LD3320_main(void)
{
	uint8 nAsrRes=0;
	LD3320_Init();	  

	while(1)
	{
    	switch(nAsrStatus)
		{
			case LD_ASR_RUNING:
			case LD_ASR_ERROR:		
							  break;
			case LD_ASR_NONE:
								nAsrStatus=LD_ASR_RUNING;
								if (RunASR()==0)	//	启动一次ASR识别流程：ASR初始化，ASR添加关键词语，启动ASR运算
								{		
									nAsrStatus = LD_ASR_ERROR;
								}
								break;

			case LD_ASR_FOUNDOK:
								 nAsrRes = LD_GetResult( );	//识别成功自动 获取识别码,识别码在LDChip.h文件中，自行定义的数据										 									
								 PrintComBit(USART1,nAsrRes ); //串口输出识别码								
								 //Board_text(nAsrRes );//开发板测试演示部分---对识别码的进行判断做出动作 (用户亦可以添加自己的功能，例如控制IO口输出，串口数据输出等)		
								 nAsrStatus = LD_ASR_NONE;
								break;
			
			case LD_ASR_FOUNDZERO:
			default: nAsrStatus = LD_ASR_NONE;
					 break;
			}//switch	
        //开发板测试
		Board_text(nAsrRes );
	}// while
}


static void Board_text(uint8 Code_Val)
{																					 
	switch(Code_Val)  //对结果执行相关操作
	{
		case CODE_DD:	//命令“打开客厅灯”
		LED_KT=1;
        printf("打开客厅灯\r\n");
		break;
		
        case CODE_GD:		//命令“关闭客厅灯”
	    LED_KT=0;
        printf("关闭客厅灯\r\n");
        break;
        
        case CODE_QDD:		//命令“打开卧室灯”
	    //打开卧室灯
        LED_WS = 1;
        printf("打开卧室灯\r\n");
        break;
        
        case CODE_DG:		//命令“关闭卧室灯”
	    //关闭卧室灯
        LED_WS = 0;
        printf("关闭卧室灯\r\n");
		break;
        
        case CODE_LSD:		//命令“全部打开”
	    //全部打开
        LED_KT=1;
        LED_WS = 1;
        printf("全部打开\r\n");
		break;
        
        case CODE_SSD:		//命令“全部关闭”
	    //全部关闭
        LED_KT=0;  
        LED_WS = 0;
        printf("全部关闭\r\n");
        break;
        
        case CODE_DDR:		//命令“打开风扇”
	    //打开风扇
        printf("打开风扇\r\n");
        JDY_FS = 0;
		break;
        
        case CODE_RDD:		//命令“关闭风扇”
	    //关闭风扇
        JDY_FS = 1;
        printf("关闭风扇\r\n");
		break;
		default:break;
	}	
}
/***********************************************************
* 名    称：LD3320_Init(void)
* 功    能：模块驱动端口初始配置
* 入口参数：  
* 出口参数：
* 说    明：
* 调用方法： 
**********************************************************/ 
void LD3320_Init(void)
{
	LD3320_GPIO_Cfg();	
	LD3320_EXTI_Cfg();
	LD3320_Spi_cfg();	 
	LD_reset();
	
}
/***********************************************************
* 名    称：	RunASR(void)
* 功    能：	运行ASR
* 入口参数：  
* 出口参数：
* 说    明：
* 调用方法： 
**********************************************************/ 
uint8 RunASR(void)
{
	uint8 i=0;
	uint8 asrflag=0;
	for (i=0; i<5; i++)			//	防止由于硬件原因导致LD3320芯片工作不正常，所以一共尝试5次启动ASR识别流程
	{
		LD_AsrStart();			//初始化ASR
		LD3320_delay(100);
		if (LD_AsrAddFixed()==0)	//添加关键词语到LD3320芯片中
		{
			LD_reset();			//	LD3320芯片内部出现不正常，立即重启LD3320芯片
			LD3320_delay(50);			//	并从初始化开始重新ASR识别流程
			continue;
		}

		LD3320_delay(10);

		if (LD_AsrRun() == 0)
		{
			LD_reset();			//	LD3320芯片内部出现不正常，立即重启LD3320芯片
			LD3320_delay(50);			//	并从初始化开始重新ASR识别流程
			continue;
		}

		asrflag=1;
		break;					//	ASR流程启动成功，退出当前for循环。开始等待LD3320送出的中断信号
	}

	return asrflag;
}

/***********************************************************
* 名    称： void Delay_( int i)
* 功    能： 短延时
* 入口参数：  
* 出口参数：
* 说    明：
* 调用方法： 
**********************************************************/ 
void Delay_( int i)
 {     
    while( i--)
       {	

        }
 }	
/***********************************************************
* 名    称：	LD3320_delay(unsigned long uldata)
* 功    能：	长延时函数
* 入口参数：  
* 出口参数：
* 说    明：
* 调用方法： 
**********************************************************/ 
 void  LD3320_delay(unsigned long uldata)
{
	unsigned int j  =  0;
	unsigned int g  =  0;
	for (j=0;j<5;j++)
	{
		for (g=0;g<uldata;g++)
		{
			Delay_(120);
		}
	}
}


/***********************************************************
* 名    称：LD3320_GPIO_Cfg(void)
* 功    能：初始化需要用到的IO口
* 入口参数：  
* 出口参数：
* 说    明：
* 调用方法： 
**********************************************************/ 
void LD3320_GPIO_Cfg(void)
{	
	    GPIO_InitTypeDef GPIO_InitStructure;
		// 配置PA8 输出	 8M 波形	
		//定义RST/A0/CS/WR端口
	 {
	 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB ,ENABLE);
		//LD_CS	/RSET
		GPIO_InitStructure.GPIO_Pin =GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_Init(GPIOB,&GPIO_InitStructure);
		GPIO_SetBits(GPIOB,GPIO_Pin_7);	/*A0默认拉高*/
	 }
}
/***********************************************************
* 名    称：LD3320_Spi_cfg(void)
* 功    能：配置SPI功能和端口初始化
* 入口参数：  
* 出口参数：
* 说    明：
* 调用方法： 
**********************************************************/ 
void LD3320_Spi_cfg(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
  	SPI_InitTypeDef  SPI_InitStructure;

	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );//PORTB时钟使能 
	RCC_APB1PeriphClockCmd(	RCC_APB1Periph_SPI2,  ENABLE );//SPI2时钟使能 	
 
	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //PB13/14/15复用推挽输出 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	LD_CS_H();
	
 	GPIO_SetBits(GPIOB,GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);  //PB13/14/15上拉

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//设置SPI工作模式:设置为主SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//设置SPI的数据大小:SPI发送接收8位帧结构
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		//选择了串行时钟的稳态:时钟悬空高
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;	//数据捕获于第二个时钟沿
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;		//定义波特率预分频的值:波特率预分频值为16
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRC值计算的多项式
	SPI_Init(SPI2, &SPI_InitStructure);  //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器
 
	SPI_Cmd(SPI2, ENABLE); //使能SPI外设
}
/***********************************************************
* 名    称： LD3320_EXTI_Cfg(void) 
* 功    能： 外部中断功能配置和相关端口配置
* 入口参数：  
* 出口参数：
* 说    明：
* 调用方法： 
**********************************************************/ 
void LD3320_EXTI_Cfg(void)
{
  EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;

  //定义IRQ中断引脚配置
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  GPIO_InitStructure.GPIO_Pin =GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	//外部中断线配置
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource3);
  EXTI_InitStructure.EXTI_Line = EXTI_Line3;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger =EXTI_Trigger_Falling;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
  EXTI_GenerateSWInterrupt(EXTI_Line3);
		
	GPIO_SetBits(GPIOA,GPIO_Pin_3);	 //默认拉高中断引脚

	EXTI_ClearFlag(EXTI_Line3);
	EXTI_ClearITPendingBit(EXTI_Line3);
	//中断嵌套配置
  NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}


/***********************************************************
* 名    称：  EXTI1_IRQHandler(void)
* 功    能： 外部中断函数
* 入口参数：  
* 出口参数：
* 说    明：
* 调用方法： 
**********************************************************/ 
void EXTI3_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line3)!= RESET ) 
	{
 		ProcessInt0();
		EXTI_ClearFlag(EXTI_Line3);
		EXTI_ClearITPendingBit(EXTI_Line3);
	}
}

