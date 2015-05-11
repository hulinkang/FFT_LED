/*------------------------------------------------------------------
 *
 *	LED32x16.h
 *	
 *Description:
 *		32x16led模块的头文件	
 *Author:
 *		hulinkang
 *Date:
 *		2015-4-22
 *------------------------------------------------------------------*/
#ifndef _LED32X16_H
#define _LED32X16_H

#include "main.h"

sbit IA = P0^0;
sbit IB = P0^1;
sbit IC = P0^2;
sbit ID = P0^3;

sbit RED   = P0^4;
sbit GREEN = P0^5;
sbit STCP  = P0^6;
sbit SHCP  = P0^7;

sbit OE = P1^3;//后期调整为PWM调整亮度


#define line15    {IA=0;IB=0;IC=0;ID=0;}
#define line14    {IA=1;IB=0;IC=0;ID=0;}
#define line13    {IA=0;IB=1;IC=0;ID=0;}
#define line12    {IA=1;IB=1;IC=0;ID=0;}
#define line11    {IA=0;IB=0;IC=1;ID=0;}
#define line10    {IA=1;IB=0;IC=1;ID=0;}
#define  line9    {IA=0;IB=1;IC=1;ID=0;}
#define  line8    {IA=1;IB=1;IC=1;ID=0;}
#define  line7    {IA=0;IB=0;IC=0;ID=1;}
#define  line6    {IA=1;IB=0;IC=0;ID=1;}
#define  line5    {IA=0;IB=1;IC=0;ID=1;}
#define  line4    {IA=1;IB=1;IC=0;ID=1;}
#define  line3    {IA=0;IB=0;IC=1;ID=1;}
#define  line2    {IA=1;IB=0;IC=1;ID=1;}
#define  line1    {IA=0;IB=1;IC=1;ID=1;}
#define  line0    {IA=1;IB=1;IC=1;ID=1;}

void sendData(uint8 );
void scan(unsigned char );

#endif