#include <REGX52.H>

sbit MotorR=P1^2;	//右电机
sbit MotorL=P1^3;	//左电机
sbit fmq=P1^0;		//蜂鸣器，低电平响
sbit LED=P1^1;		//LED指示灯，低电平亮
sbit Trig=P2^0;		//超声波Trig
sbit Echo=P2^1;		//超声波Echo

unsigned char Counter,CompareR,CompareL;	//计数值和左右比较值，用于输出PWM
unsigned char KeyNum,Speed;			//按键号和当前速度
int quanshu=1;					//圈数，1就是第一圈
unsigned char juli;				//超声波测到的距离，单位cm
bit likai;					//1=已经离开过A点起跑线
bit tingguo;					//1=任务3已经等停过一次

/*---------------- 延时，从 Delay.c 搬过来，没改 ----------------*/
void Delay(unsigned int xms)
{
	unsigned char i, j;
	while(xms--)
	{
		i = 2;
		j = 239;
		do
		{
			while (--j);
		} while (--i);
	}
}

/*---------------- 独立按键，从 Key.c 搬过来 ----------------
  注意：原来写的是 P3_2、P3_3，这两个脚被八路巡线的探头占了，
  所以键3、键4改到 P2_5、P2_4，要是你的按键还是接在 P3_2、P3_3，
  把这两行改回去就行。
*/
unsigned char Key()
{
	unsigned char KeyNumber=0;
	
	if(P2_7==0){Delay(20);while(P2_7==0);Delay(20);KeyNumber=1;}
	if(P2_6==0){Delay(20);while(P2_6==0);Delay(20);KeyNumber=2;}
	if(P2_5==0){Delay(20);while(P2_5==0);Delay(20);KeyNumber=3;}
	if(P2_4==0){Delay(20);while(P2_4==0);Delay(20);KeyNumber=4;}
	
	return KeyNumber;
}

/*---------------- 定时器0初始化，从 Timer0.c 搬过来，没改 ----------------*/
void Timer0_Init()
{
	TMOD &= 0xF0;		//设置定时器模式
	TMOD |= 0x01;		//设置定时器模式
	TL0 = 0x9C;		//设置定时初值
	TH0 = 0xFF;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
	ET0=1;
	EA=1;
	PT0=0;
}

/*---------------- 声光提示 ----------------*/
void fengmingqi()		//长鸣一声，原来的写法一点没动
{
	fmq=0;
	Delay(1000);
	fmq=1;
}

void duanming()			//短鸣一声，起点用
{
	fmq=0;
	Delay(100);
	fmq=1;
}

void shandeng()			//停车以后LED慢闪，按任意键回主菜单重新选任务
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

/*---------------- 循迹走一步，判断顺序还是原来那一套 ----------------*/
void xunji()
{
	if(P3_0==0 ||P3_1==0||P3_2==0 )
	{
		CompareR=0;		//右轮不给电，右转
		CompareL=Speed;
	}
	else if(P3_5==0 ||P3_6==0||P3_7==0 )
	{
		CompareR=Speed;		//左轮不给电，左转
		CompareL=0;
	}
	else
	{
		CompareR=Speed;		//两边都给电，直行
		CompareL=Speed;
	}
}

/*---------------- 超声波，用定时器1量回波时间，定时器0还要留着出PWM ----------------*/
void chaoshengbo_init()
{
	TMOD &= 0x0F;		//只改定时器1，定时器0的模式不动
	TMOD |= 0x10;
	TR1=0;
	Trig=0;
}

unsigned char chaoshengbo()	//测一次距离，结果放在juli里，也返回出去
{
	unsigned int n;
	
	Trig=1;
	Delay(1);		//给个大于10us的高电平
	Trig=0;
	
	n=0;
	while(Echo==0)		//等回波开始
	{
		n++;
		if(n>5000)	//等不到回波，就当前面没有东西
		{
			juli=99;
			return juli;
		}
	}
	
	TH1=0;
	TL1=0;
	TR1=1;			//开始量高电平有多长
	n=0;
	while(Echo==1)
	{
		n++;
		if(n>5000)	//回波太长，不量了
		{
			break;
		}
	}
	TR1=0;
	
	juli=(TH1*256+TL1)/58;	//12M晶振，一个计数就是1us，除以58得厘米
	return juli;
}

/*---------------- 任务1：外圈跑一圈，回A点停车 ----------------*/
void renwu1()
{
	Speed=100;
	CompareR=Speed;
	CompareL=Speed;
	LED=0;			//行驶中LED常亮
	likai=0;
	duanming();		//起点短鸣1声
	while(1)
	{
		xunji();
		if(P3==0)	//八个探头全压黑线，就是压到A点的起终点线了
		{
			if(likai==1)	//之前已经离开过起点，说明这一圈跑完了
			{
				CompareR=0;
				CompareL=0;	//停车
				fengmingqi();	//终点长鸣1声
				LED=1;
				shandeng();	//停车后LED慢闪
				return;
			}
		}
		else
		{
			likai=1;	//没压线，说明车已经离开起点了
		}
	}
}

/*---------------- 任务2：外圈--内圈--外圈，三圈 ----------------*/
void renwu2()
{
	Speed=100;
	CompareR=Speed;
	CompareL=Speed;
	quanshu=1;
	likai=0;
	LED=1;			//第一圈外圈，灯不亮
	duanming();		//起点短鸣1声
	while(1)
	{
		xunji();
		
		/* B点分岔：最右边探头和中间探头一起压线，说明到岔口了 */
		if((P3_0==0)&&(P3_3==0||P3_4==0))
		{
			if(quanshu==2)		//第二圈走内圈，要提前往右拐
			{
				CompareR=0;	//右轮停，右转
				CompareL=Speed;
				Delay(300);	//这个时间按场地调，拐早了就改小，拐不过来就改大
			}
		}
		
		if(P3==0)	//A点起终点线
		{
			if(likai==1)
			{
				quanshu++;		//过一次A点就多算一圈
				if(quanshu==2)		//第二圈开始走内圈，声光提示
				{
					LED=0;		//内圈LED亮
					duanming();	//蜂鸣器响一下
				}
				if(quanshu==3)		//第三圈又走外圈，灯灭
				{
					LED=1;
				}
				if(quanshu==4)		//三圈跑完，A点停车
				{
					CompareR=0;
					CompareL=0;
					fengmingqi();	//长鸣1声
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

/*---------------- 任务3：认等停指示，停5秒再自动接着走 ----------------*/
void renwu3()
{
	unsigned char i;
	
	Speed=100;
	CompareR=Speed;
	CompareL=Speed;
	LED=0;			//行驶中LED常亮
	likai=0;
	tingguo=0;
	duanming();		//起点短鸣1声
	while(1)
	{
		xunji();
		if(P3==0)	//压到横线了
		{
			if(likai==1)	//已经离开过A点，这条线才算数
			{
				if(tingguo==0)		//第一次压线，是E点那两条等停指示
				{
					CompareR=0;
					CompareL=0;	//马上停，不许越过第二条线
					for(i=0;i<25;i++)	//25*200ms=5秒，期间蜂鸣器叫、LED闪
					{
						fmq=0;
						LED=0;
						Delay(100);
						fmq=1;
						LED=1;
						Delay(100);
					}
					tingguo=1;	//记下来，下次压线就是回A点
					Speed=100;	//5秒到了，自动接着走，不用按键
				}
				else			//第二次压线，是A点终点线
				{
					CompareR=0;
					CompareL=0;
					fengmingqi();	//长鸣1声
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

/*---------------- 任务4：超声波避障，障碍拿走自动接着走 ----------------*/
void renwu4()
{
	Speed=100;
	CompareR=Speed;
	CompareL=Speed;
	LED=0;			//行驶中LED常亮
	likai=0;
	chaoshengbo_init();
	duanming();		//起点短鸣1声
	while(1)
	{
		xunji();
		
		chaoshengbo();
		if(juli<30)		//30cm以内开始减速
		{
			Speed=50;
		}
		else
		{
			Speed=100;
		}
		
		if(juli<20)		//20cm以内停车，声光报警
		{
			Speed=0;
			CompareR=0;
			CompareL=0;
			while(1)
			{
				fmq=0;		//5Hz左右，响100ms停100ms
				LED=0;		//LED快闪
				Delay(100);
				fmq=1;
				LED=1;
				Delay(100);
				chaoshengbo();
				if(juli>30)	//障碍物拿走了，自动接着走
				{
					Speed=100;
					break;
				}
			}
		}
		
		if(P3==0)	//A点起终点线，跑完一圈停车
		{
			if(likai==1)
			{
				CompareR=0;
				CompareL=0;
				fengmingqi();	//停车后长鸣
				LED=0;		//停车后LED常亮
				while(1);
			}
		}
		else
		{
			likai=1;
		}
	}
}

/*---------------- 总入口，原来是调速度，现在用来挑任务 ----------------
  键1=任务1  键2=任务2  键3=任务3  键4=任务4  （也就是题目说的四个档位）
*/
void tiaosu()
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
	TL0 = 0x9C;		//设置定时初值
	TH0 = 0xFF;		//设置定时初值
	Counter++;
	Counter%=100;	//计数值变化范围限制在0~99
	if(Counter<CompareR)	//计数值小于右轮比较值
	{
		MotorR=1;
	}
	else
	{
		MotorR=0;
	}
	if(Counter<CompareL)	//计数值小于左轮比较值
	{
		MotorL=1;
	}
	else
	{
		MotorL=0;
	}
}
