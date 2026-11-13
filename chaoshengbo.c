#include "global.h"

void chaoshengbo_init()		//超声波，定时器1计时
{
	TMOD &= 0x0F;
	TMOD |= 0x10;
	TR1=0;
	Trig=0;
}

unsigned char chaoshengbo()	//测距
{
	unsigned int n;
	
	Trig=1;
	Delay(1);
	Trig=0;
	
	n=0;
	while(Echo==0)		//等回波
	{
		n++;
		if(n>400)	//没有障碍
		{
			juli=99;
			return juli;
		}
	}
	
	TH1=0;
	TL1=0;
	TR1=1;
	n=0;
	while(Echo==1)
	{
		n++;
		if(n>400)
		{
			break;
		}
	}
	TR1=0;
	
	juli=(TH1*256+TL1)/58;	//1us对应1/58cm
	return juli;
}
