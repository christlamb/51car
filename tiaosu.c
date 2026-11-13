#include "global.h"

static unsigned char LineOffCnt=0;
static unsigned char LineReady=1;

//定时器0，100us@11.0592MHz
void Timer0_Init()
{
	TMOD &= 0xF0;
	TMOD |= 0x01;
	TL0 = 0x9C;
	TH0 = 0xFF;
	TF0 = 0;
	TR0 = 1;
	ET0=1;
	EA=1;
	PT0=0;
}

void tiaosu()			//按键选择
{
	Timer0_Init();
	Speed=0;
	CompareR=0;
	CompareL=0;
	LED=1;
	while(1)
	{
		KeyNum=Key();
		if(KeyNum==1)
		{
			renwu1();
		}
		else if(KeyNum==2)
		{
			renwu2();
		}
		else if(KeyNum==3)
		{
			renwu3();
		}
		else if(KeyNum==4)
		{
			renwu4();
		}
	}
}

void Timer0_Routine() interrupt 1
{
	TL0 = 0x9C;
	TH0 = 0xFF;
	Counter++;
	Counter%=100;
	if(Counter<CompareR)	//右轮
	{
		MotorR=1;
	}
	else
	{
		MotorR=0;
	}
	if(Counter<CompareL)	//左轮
	{
		MotorL=1;
	}
	else
	{
		MotorL=0;
	}
	if(P3==0)
	{
		if(LineReady==1)
		{
			A_Line_Flag=1;
			LineReady=0;
		}
		A_Line_On=1;
		LineOffCnt=0;
	}
	else
	{
		A_Line_On=0;
		if(LineOffCnt<255)
		{
			LineOffCnt++;
		}
		if(LineOffCnt>30)
		{
			LineReady=1;
		}
	}
}
