/*------------------------------------------------------------------
 *
 *	main.c
 *	
 *Description:
 *		16*32LED点阵音乐频谱，快速傅里叶变换
 *		
 *Author:
 *		hulinkang
 *Date:
 *		2015-4-20
 *------------------------------------------------------------------*/
#include "main.h"
#include "FFT.h"
#include "LED32x16.h"

/*私有函数申明*/
void Timer0Init(void);
void PWM_duty(uint8 );

/*定义全局变量*/
uint8 Line = 0;

void main(void)
{
	InitADC();
	Timer0Init();
	PWM_duty(50);//led点阵屏幕50%亮度
	EA = 1; 	
	//OE = 0;
	while(1){
		FFT();//耗时60ms
	}
}

/*定时器初始化，定时3ms,3ms内完成一行led屏显示*/
void Timer0Init(void)		//30ms@24.000MHz
{
	AUXR &= 0x7F;		//定时器时钟12T模式
	TMOD &= 0xF0;		//设置定时器模式
	TMOD |= 0x01;		//设置定时器模式
	TL0 = (65536 - OSC_FREQ / OSC_PER_INST / 1000 * 3);	
	TH0 = (65536 - OSC_FREQ / OSC_PER_INST / 1000 * 3) >> 8;
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器T0开始运行
	
	ET0 = 1;
}

/*PWM亮度调节函数*/
void PWM_duty(uint8 n)//11.7KHz,百分比占空比,P1.3引脚
{
	uint8 i = 0;
	CCON = 0;             	//Initial PCA control register
							//PCA timer stop running
							//Clear CF flag
                            //Clear all module interrupt flag
    CL = 0;           		//Reset PCA base timer
    CH = 0;
    CMOD = 0x0e;           	//Set PCA timer clock source as Fosc/8  3MHz
							//Disable PCA timer overflow interrupt
	i = n*255/100;//高
	
    CCAP0H = CCAP0L = 255 - i;  //PWM0 port output 50% duty cycle square wave
    CCAPM0 = 0x42;      //PCA module-0 work in 8-bit PWM mode and no PCA interrupt

    CR = 1;         	//PCA timer start run
}


void Timer0_Isr(void) interrupt ISR_T0
{
	TL0 = (65536 - OSC_FREQ / OSC_PER_INST / 1000 * 3);	
	TH0 = (65536 - OSC_FREQ / OSC_PER_INST / 1000 * 3) >> 8;
	sendData(Line);
	scan(Line);
	STCP=1;
	Line++;
	Line = Line % 16;
	STCP=0;
}

	
	
