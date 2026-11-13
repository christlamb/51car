#include "global.h"

void fengmingqi()		//长鸣1声
{
	fmq=0;
	Delay(1000);
	fmq=1;
}

void duanming()			//短鸣1声
{
	fmq=0;
	Delay(100);
	fmq=1;
}

void shandeng()			//停车后慢闪
{
	CompareR=0;
	CompareL=0;
	while(1)
	{
		LED=0;
		Delay(500);
		LED=1;
		Delay(500);
		if(Key()!=0)
		{
			return;
		}
	}
}

void xunji()			//循迹
{
	if((P3_0==0 ||P3_1==0||P3_2==0 )&&(P3_7==1))
	{
		CompareR=0;		//右转
		CompareL=Speed;
	}
	else if((P3_5==0 ||P3_6==0||P3_7==0 )&&(P3_0==1))
	{
		CompareR=Speed;		//左转
		CompareL=0;
	}
	else
	{
		CompareR=Speed;		//直行
		CompareL=Speed;
	}
}
