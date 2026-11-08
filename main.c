#include <REGX52.H>

sbit MotorR=P1^2;	//右电机
sbit MotorL=P1^3;	//左电机
sbit fmq=P1^0;		//蜂鸣器
sbit LED=P1^1;		//LED
sbit Trig=P2^0;		//超声波Trig
sbit Echo=P2^1;		//超声波Echo

unsigned char Counter,CompareR,CompareL;	//PWM计数值和左右比较值
unsigned char KeyNum,Speed;			//按键号、速度
int quanshu=1;					//圈数
unsigned char juli;				//超声波距离，cm
bit likai;					//已离开A点
bit tingguo;					//已等停过

void Delay(unsigned int xms)
{
	unsigned char i, j;
	while(xms--)
	{
		i = 2;
		j = 201;
		do
		{
			while (--j);
		} while (--i);
	}
}

//键1~4：P2_7、P2_6、P2_5、P2_4
unsigned char Key()
{
	unsigned char KeyNumber=0;
	
	if(P2_7==0){Delay(20);while(P2_7==0);Delay(20);KeyNumber=1;}
	if(P2_6==0){Delay(20);while(P2_6==0);Delay(20);KeyNumber=2;}
	if(P2_5==0){Delay(20);while(P2_5==0);Delay(20);KeyNumber=3;}
	if(P2_4==0){Delay(20);while(P2_4==0);Delay(20);KeyNumber=4;}
	
	return KeyNumber;
}

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
		if(n>5000)	//没有障碍
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
		if(n>5000)
		{
			break;
		}
	}
	TR1=0;
	
	juli=(TH1*256+TL1)/58;	//1us对应1/58cm
	return juli;
}

void renwu1()			//任务1：外圈一圈
{
	Speed=100;
	CompareR=Speed;
	CompareL=Speed;
	LED=0;
	likai=0;
	duanming();
	while(1)
	{
		if(P3==0)	//全黑，压到A点线
		{
			CompareR=0;
			CompareL=0;
			fengmingqi();
			LED=1;
			shandeng();
			return;
		}
		else if((P3_3==0 || P3_4==0)&&(P3_7==0||P3_6==0)&&P3_0==1)	//分叉
		{
			CompareR=Speed;
			CompareL=Speed;
			Delay(300);	//直走时间
		}
		else if(P3_0==0 ||P3_1==0||P3_2==0 )
		{
			CompareR=0;		//右转
			CompareL=Speed;
		}
		else if(P3_5==0 ||P3_6==0||P3_7==0 )
		{
			CompareR=Speed;		//左转
			CompareL=0;
		}

		else
		{
			CompareL=Speed;
			CompareR=Speed;
		}


	}
}

void renwu2()			//任务2：外圈-内圈-外圈
{
	Speed=100;
	CompareR=Speed;
	CompareL=Speed;
	quanshu=1;
	//likai=0;
	LED=1;
	duanming();
	while(1)
	{	
	yikaishi:
		if(P3==0)	//A点线
		{
			while(P3==0)
			{
				CompareL=Speed;
				CompareR=Speed;
			}
			quanshu++;
			if(quanshu==2)		//第二圈走内圈
			{
				LED=0;
				duanming();
				goto yikaishi;
			}
			else if(quanshu==3)		//第三圈走外圈
			{
				LED=1;

			}
			else if(quanshu==4)		//三圈跑完
			{
				CompareR=0;
				CompareL=0;
				fengmingqi();
				LED=1;
				shandeng();
				return;
			}
		
		}

		else if((P3_3==0 || P3_4==0)&&(P3_7==0||P3_6==0)&&P3_0==1)	//B点分岔
		{
			if(quanshu==2)			//内圈
			{
				CompareL=0;
				CompareR=Speed;
				Delay(200);	//转弯时间
			}
			else
			{
				CompareL=Speed;
				CompareR=Speed;
			}
		}
		

		else
		{
			xunji();
		}
	}
}

void renwu3()			//任务3：等停指示，停5秒再走
{
	unsigned char i;
	
	Speed=100;
	CompareR=Speed;
	CompareL=Speed;
	LED=0;
	likai=0;
	tingguo=0;
	duanming();
	while(1)
	{
		xunji();
		if(P3==0)
		{
			if(likai==1)
			{
				if(tingguo==0)		//第一次是等停线
				{
					CompareR=0;
					CompareL=0;
					for(i=0;i<25;i++)	//5秒，声光提示
					{
						fmq=0;
						LED=0;
						Delay(100);
						fmq=1;
						LED=1;
						Delay(100);
					}
					tingguo=1;
					Speed=100;
				}
				else			//第二次是A点线
				{
					CompareR=0;
					CompareL=0;
					fengmingqi();
					LED=1;
					shandeng();
					return;
				}
			}
		}
		else
		{
			likai=1;
		}
	}
}

void renwu4()			//任务4：超声波避障
{
	Speed=100;
	CompareR=Speed;
	CompareL=Speed;
	LED=0;
	likai=0;
	chaoshengbo_init();
	duanming();
	while(1)
	{
		xunji();
		
		chaoshengbo();
		if(juli<30)		//30cm内减速
		{
			Speed=50;
		}
		else
		{
			Speed=100;
		}
		
		if(juli<20)		//20cm内停车报警
		{
			Speed=0;
			CompareR=0;
			CompareL=0;
			while(1)
			{
				fmq=0;
				LED=0;
				Delay(100);
				fmq=1;
				LED=1;
				Delay(100);
				chaoshengbo();
				if(juli>30)	//障碍移除继续
				{
					Speed=100;
					break;
				}
			}
		}
		
		if(P3==0)		//A点线，跑完一圈
		{
			if(likai==1)
			{
				CompareR=0;
				CompareL=0;
				fengmingqi();
				LED=0;
				while(1);
			}
		}
		else
		{
			likai=1;
		}
	}
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

void main()
{
	MotorR=0;
	MotorL=0;
	
	tiaosu();
	
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
}
