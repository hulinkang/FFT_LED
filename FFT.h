/*-----------------------------------------------------------
 *		FFT.h文件
 *
 *Description：
 *		快速傅里叶变换的头文件
 *Author：
 *		hulinkang
 *Data：
 *		2015-4-20
 *----------------------------------------------------------*/
 
#ifndef _FFT_H
#define _FFT_H

#include "main.h"


/*-------------------------------------------------------------------------------------------------------
	幅频特性是周期性的偶函数，FFT之后会发现图形是对称的。所以FFT变换得到的数据一般要去掉一半。
	横坐标频率的最大值是采样频率，每个点对应的f(k) = [n/(N/2)]*fs  (fs采样频率，N采样点数，n是第n个)
---------------------------------------------------------------------------------------------------------*/
#define SAMPLE_NUM 	128
#define LAYER		7	//2的LAYER次方等于 SAMPLE_NUM
#define FREQ_TAB	32	//32*16的点阵屏幕，32个频率点
#define RESULT_NUM  64	//SAMPLE_NUM的一半

/*ADC转换通道 P1.0口*/
#define ADC_CHANNEL 0
 
/*Define ADC operation const for ADC_CONTR*/
#define ADC_POWER   0x80            //ADC power control bit
#define ADC_FLAG    0x10            //ADC complete flag
#define ADC_START   0x08            //ADC start control bit
#define ADC_SPEEDLL 0x00            //420 clocks
#define ADC_SPEEDL  0x20            //280 clocks
#define ADC_SPEEDH  0x40            //140 clocks
#define ADC_SPEEDHH 0x60            //70 clocks


void FFT(void);
uint16 sqrt_16(uint32 );

void InitADC(void);
int16 GetADCResult(void);
static void Delay(uint16 );


#endif
 