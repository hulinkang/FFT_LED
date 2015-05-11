/*-----------------------------------------------------------
 *		FFT.c文件
 *
 *Description：
 *		快速傅里叶变换以及AD采样部分的函数
 *Author：
 *		hulinkang
 *Data：
 *		2015-4-20
 *----------------------------------------------------------*/
 
//fft函数，传入AD结果，传出变化结果
#include "FFT.h"

//放大128倍后的sin整数表（128）
code int8 sin_tab[128] = { 0, 6, 12, 18, 24, 30, 36, 42, 48, 54, 59, 65, 70, 75, 80, 85, 89, 94, 98, 102, 

105, 108, 112, 114, 117, 119, 121, 123, 124, 125, 126, 126, 126, 126, 126, 125, 124, 123, 121, 119, 117, 114, 112, 

108, 105, 102, 98, 94, 89, 85, 80, 75, 70, 65, 59, 54, 48, 42, 36, 30, 24, 18, 12, 6, 0, -6, -12, -18, -24, -30, 

-36, -42, -48, -54, -59, -65, -70, -75, -80, -85, -89, -94, -98, -102, -105, -108, -112, -114, -117, -119, -121, 

-123, -124, -125, -126, -126, -126, -126, -126, -125, -124, -123, -121, -119, -117, -114, -112, -108, -105, -102, 

-98, -94, -89, -85, -80, -75, -70, -65, -59, -54, -48, -42, -36, -30, -24, -18, -12, -6 };

//放大128倍后的cos整数表（128）
code int8 cos_tab[128] = { 127, 126, 126, 125, 124, 123, 121, 119, 117, 114, 112, 108, 105, 102, 98, 94, 

89, 85, 80, 75, 70, 65, 59, 54, 48, 42, 36, 30, 24, 18, 12, 6, 0, -6, -12, -18, -24, -30, -36, -42, -48, -54, -59, 

-65, -70, -75, -80, -85, -89, -94, -98, -102, -105, -108, -112, -114, -117, -119, -121, -123, -124, -125, -126, -

126, -126, -126, -126, -125, -124, -123, -121, -119, -117, -114, -112, -108, -105, -102, -98, -94, -89, -85, -80, 

-75, -70, -65, -59, -54, -48, -42, -36, -30, -24, -18, -12, -6, 0, 6, 12, 18, 24, 30, 36, 42, 48, 54, 59, 65, 70, 

75, 80, 85, 89, 94, 98, 102, 105, 108, 112, 114, 117, 119, 121, 123, 124, 125, 126, 126 };

//采样存储序列表,二进制倒序
code int8 BR_tab[128] = {
0, 64, 32, 96, 16, 80, 48, 112,
8, 72, 40, 104, 24, 88, 56, 120,
4, 68, 36, 100, 20, 84, 52, 116,
12, 76, 44, 108, 28, 92, 60, 124,
2, 66, 34, 98, 18, 82, 50, 114,
10, 74, 42, 106, 26, 90, 58, 122,
6, 70, 38, 102, 22, 86, 54, 118,
14, 78, 46, 110, 30, 94, 62, 126,
1, 65, 33, 97, 17, 81, 49, 113,
9, 73, 41, 105, 25, 89, 57, 121,
5, 69, 37, 101, 21, 85, 53, 117,
13, 77, 45, 109, 29, 93, 61, 125,
3, 67, 35, 99, 19, 83, 51, 115,
11, 75, 43, 107, 27, 91, 59, 123,
7, 71, 39, 103, 23, 87, 55, 119,
15, 79, 47, 111, 31, 95, 63, 127
};

/*-------------------------------------------------------------------------------
 *FFT变换得到的64个有效频率值分布在点阵32个列；
 *result[0]表示的是直流分量，result[63]是20Khz信号；
 *缺点：分辨率太低，导致低频部分分辨不开。
 *这是尽量做到最好的效果了
 *---------------------------------------------------------------------------*/
code uint8 display_tab[FREQ_TAB] = {
 1,  2,  3,  4,  5,  6,  7,  8, 
 9, 10, 11, 12, 13, 14, 16, 17,
18, 20, 22, 24, 26, 28, 31, 34,
37, 40, 43, 47, 52, 56, 60, 63
};

xdata volatile uint8   result[FREQ_TAB];//32*16点阵上显示的频率柱，绿色柱，32个值对应32列绿色柱的高度
xdata volatile uint8 topPoint[FREQ_TAB];//32*16点阵上显示的漂浮点，红色点，32个值对应32列红色点的高度

xdata int16 FftReal[SAMPLE_NUM];//存储FFT实
xdata int16 FftImage[SAMPLE_NUM];//存储FFT虚


/*----------------------------------------------------------------------
 *函数名：FFT函数
 *		
 *输入：无。内部调用了GetADCResult()函数获得AD参数
 *输出：无。FFT转换的结果存储在result[32]数组中。
-----------------------------------------------------------------------*/
void FFT(void)
{
    uint8 i,bb,j,k,p;
    int16 TR,TI,temp;      
	uint8 temp_result = 0;
       	
    for(i = 0; i < SAMPLE_NUM; i++) {
		TL1 = (65536 - OSC_FREQ / OSC_PER_INST / 1000 * 25 /1000);	//25Us，对应40KHz采样频率
		TH1 = (65536 - OSC_FREQ / OSC_PER_INST / 1000 * 25 /1000) >> 8;
		TF1 = 0;
		TR1 = 1;        //定时器1开始计时
		while(TF1 == 0)
			;			//计时25us
		EA = 0;			//关闭总中断，STC单片机AD时候推荐关闭中断
		FftReal[BR_tab[i]] = (int16)(GetADCResult());
		EA = 1;			//开启总中断
		FftImage[i] = 0;
		TR1 = 0;    	//定时器1停止计时
    } 	
	
	
	//------------------FFT开始----------------------------
    for(i = 1; i <= LAYER; i++) { //LAYER次迭代过程
        bb = 1;
        bb <<= (i-1);                                      
        for(j = 0; j <= bb - 1; j++) { //完成因子Wnp的变化
            p = 1;
            p <<= (LAYER - i);           
            p = p * j;
            for(k = j; k < SAMPLE_NUM; k = k + 2*bb) { //完成相同Wnp的蝶形运算
                TR = FftReal[k];
				TI = FftImage[k];
				temp = FftReal[k+bb];
                FftReal[k]  = FftReal[k]  + ((FftReal[k+bb] * cos_tab[p]) >> 7) + ((FftImage[k+bb] * sin_tab[p]) >> 7);
                FftImage[k] = FftImage[k] - ((FftReal[k+bb] * sin_tab[p]) >> 7) + ((FftImage[k+bb] * cos_tab[p]) >> 7);
                FftReal[k+bb]  = TR - ((FftReal[k+bb] * cos_tab[p]) >> 7) - ((FftImage[k+bb] * sin_tab[p]) >>7);
                FftImage[k+bb] = TI + ((temp * sin_tab[p]) >> 7) - ((FftImage[k+bb] * cos_tab[p]) >>7); 
                
                FftReal[k] >>= 1;             
                FftImage[k] >>= 1; 
                FftReal[k+bb] >>= 1;                 
                FftImage[k+bb] >>= 1;                                                                               
            }  
        }
    }
	//-------------------FFT结束------------------------------
	
	//根据FFT结构计算功率和显示内容
	for(i = 0; i < FREQ_TAB; i++) {
		j = display_tab[i];//频率分布表
		temp_result = (uint8)(FftReal[j] * FftReal[j] + FftImage[j] * FftImage[j]);//求功率

        if(temp_result<4)
			temp_result=0;
        else if(temp_result<9)
			temp_result=1;
        else if(temp_result<16)
			temp_result=2;
        else if(temp_result<25)
			temp_result=3;
        else if(temp_result<36)
			temp_result=4;
        else if(temp_result<49)
			temp_result=5;
        else if(temp_result<64)
			temp_result=6;
        else if(temp_result<81)
			temp_result=7;
        else if(temp_result<100)
			temp_result=8;
        else if(temp_result<121)
			temp_result=9;
        else if(temp_result<144)
			temp_result=10;
        else if(temp_result<169)
			temp_result=11;
        else if(temp_result<196)
			temp_result=12;
        else if(temp_result<225)
			temp_result=13;
        else if(temp_result<255)
			temp_result=14;//
        else 
			temp_result=15;
		
		//更新红色漂浮点的值
		if(temp_result >= topPoint[i])
			topPoint[i] = temp_result;//顶上去最新高度
		else {
			if(result[i] < topPoint[i])
				topPoint[i] -= 1;//漂浮点下落速度1格/次
			else
				;//表示上次的绿色顶点是顶上去的，保持一次
		}
	
		//更新频率柱的值
		if(temp_result >= result[i])
			result[i] = temp_result;
		else
			if(result[i] >= 2)
				result[i] -= 2;	//下落速度2格/次
			else if(result[i] >= 1)
				result[i]  = 0; 

			
	}
}



/*----------------------------------------------------------------------------
					ADC部分
-------------------------------------------------------------------------------*/
/*----------------------------
Initial ADC sfr
----------------------------*/
void InitADC(void)
{
    P1ASF = 0x01;                   //Open 1 channels ADC function
    ADC_RES = 0;                    //Clear previous result
    ADC_CONTR = ADC_POWER | ADC_SPEEDH;
    Delay(2);                       //ADC power-on and delay
	
	AUXR &= 0xBF;		//采样定时器1设置为12T模式
	TMOD &= 0x0F;		
	TMOD |= 0x10;		
}

/*----------------------------
Get ADC result
----------------------------*/
int16 GetADCResult(void)
{
    ADC_CONTR = ADC_POWER | ADC_SPEEDH | ADC_CHANNEL | ADC_START;
    _nop_();                        //Must wait before inquiry
    _nop_();
    _nop_();
    _nop_();
    while (!(ADC_CONTR & ADC_FLAG))
		;//Wait complete flag
    ADC_CONTR &= ~ADC_FLAG;         //Close ADC

    //return ADC_RES;                 //Return ADC result
	return (ADC_RES*4 + ADC_RESL); 
}


/*----------------------------
Software delay function
----------------------------*/
void Delay(uint16 n)
{
    uint16 x;

    while (n--){
        x = 5000;
        while (x--);
    }
}
