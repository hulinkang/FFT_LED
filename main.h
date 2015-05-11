/*------------------------------------------------------------------
 *
 *	main.h
 *	
 *Description£:
 *		包含单片机相关的头文件、晶振、指令周期、数据类型、中断号。
 *Author:
 *		hulinkang
 *Date:
 *		2015-4-20
 *------------------------------------------------------------------*/

#ifndef _MAIN_H
#define _MAIN_H

/*------------------------------------------------------
 *每个项目都需要重新修改配置
 *单片机晶振
 *-----------------------------------------------------*/
#include <STC12C5A60S2.H>
#include <intrins.h>

//#define OSC_FREQ (11059200UL) 
#define OSC_FREQ (24000000UL) 

//Number of oscillations per instruction in timer
#define OSC_PER_INST (12) 

/*------------------------------------------------------
 *数据类型定义
 *-----------------------------------------------------*/
typedef unsigned char uint8;
typedef 		 char int8;
typedef unsigned int  uint16;
typedef          int  int16;
typedef unsigned long uint32;
typedef          long int32;

/*------------------------------------------------------
 *Interrupts
 *-----------------------------------------------------*/
#define ISR_INT0    0
#define ISR_T0      1
#define ISR_INT1    2
#define ISR_T1 		3
#define ISR_UART1  	4
#define ISR_ADC		5
#define ISR_LVD 	6
#define ISR_PCA		7
#define ISR_UART2	8
#define ISR_SPI		9


#endif
