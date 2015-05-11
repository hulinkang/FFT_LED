/*------------------------------------------------------------------
 *
 *	LED32x16.c
 *	
 *Description:
 *		32x16led模块显示程序
 *Author:
 *		hulinkang
 *Date:
 *		2015-4-22
 *------------------------------------------------------------------*/
#include "LED32x16.h"
#include "fft.h"

/*申明FFT.c中的全局变量*/
extern xdata volatile uint8 result[FREQ_TAB]; //红色柱状，32列
extern xdata volatile uint8 topPoint[FREQ_TAB];//绿色点，32列
	
/*-------------------------------------------------------------------
 *函数名：32x16LED显示程序
 *说明：
 *		SHCP上升沿移位送入一行数据
 *
 *输入：
 *输出：
---------------------------------------------------------------------*/
void sendData(uint8 line)
{
	uint8 i;
	for(i = 0; i < 32; i++) {
		SHCP = 0;
		
		GREEN = 1;
		RED   = 1;
		//送一行频率柱数据
		if(result[i] > line) {
			GREEN = 0;//green
			RED   = 1;
			
		}
		//送一行漂浮点数据
		if(topPoint[i] == line) {
			GREEN = 1;	//RED point
			RED   = 0;
		}
		SHCP = 1;
	}
}

/*行选*/
void scan(uint8 value)
{
	switch(value){
		case  0: line0;break;
		case  1: line1;break;
		case  2: line2;break;
		case  3: line3;break;
		case  4: line4;break;
		case  5: line5;break;
		case  6: line6;break;
		case  7: line7;break;
        case  8: line8;break;
		case  9: line9;break;
		case 10:line10;break;
		case 11:line11;break;
        case 12:line12;break;
		case 13:line13;break;
		case 14:line14;break;
	    case 15:line15;break;
		default:break;
	}
}

