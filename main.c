#include <REGX52.H>
#include "Delay.h"
#include "Key.h"
#include "Timer0.h"

sbit MotorR=P1^2;
sbit MotorL=P1^3;

unsigned char Counter,Compare;	//计数值和比较值，用于输出PWM
unsigned char KeyNum,Speed;
int quanshu=1;

void fengmingqi()
{
	P1_0=0;
	Delay(1000);
	P1_0=1;
}

void tiaosu()
{
	Timer0_Init();
	fengmingqi();
	while(1)
	{
		KeyNum=Key();
		if(KeyNum==1)
		{
			Compare=100;
			while (1)
			{
				if((P3_0==0)&&(P3_3==0||P3_4==0)&&(quanshu==2))
				{
					while((P3_0==0)&&(P3_3==0||P3_4==0))
					{
						MotorR=0;//右转
						Compare=0;
						Delay(1500);
					}
					quanshu++;				
				}
				else if((P3_0==0)&&(P3_3==0||P3_4==0)&&(quanshu==1||3))
				{
					while((P3_0==0)&&(P3_3==0||P3_4==0))
					{
						MotorR=1;
						MotorL=1;
					}
				}
				else if(P3_0==0 ||P3_1==0||P3_2==0 )
				{
					MotorR=0;//右转
				}
				else if(P3_5==0 ||P3_6==0||P3_7==0 )
				{
					MotorL=0;//左转
				}
				else if(P3==0)
				{
					Compare=0;
				}
				
				else if(P3==1)
				{
					fengmingqi();
				}

				else
				{
					MotorR=1;
					MotorL=1;
					Compare=100;
					
				}
			}				/* code */
		}
	}
}

void main()
{
	MotorR=0;
	MotorL=0;
	
	tiaosu();
	
}

void Timer0_Routine() interrupt 1
{
	TL0 = 0x9C;		//设置定时初值
	TH0 = 0xFF;		//设置定时初值
	Counter++;
	Counter%=100;	//计数值变化范围限制在0~99
	if(Counter<Compare)	//计数值小于比较值
	{
		MotorR=1;
		MotorL=1;		//输出1
	}
	else				//计数值大于比较值
	{
		MotorR=0;
		MotorL=0;			//输出0
	}
}
