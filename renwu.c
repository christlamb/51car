#include "global.h"

void renwu1()			//任务1：外圈一圈
{
	A_Line_Flag=0;
	Speed=100;
	CompareR=Speed;
	CompareL=Speed;
	LED=1;
	likai=0;
	duanming();
	while(1)
	{
		if(A_Line_Flag)	//全黑，压到A点线
		{
			A_Line_Flag=0;
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
	unsigned char neiquan;
	
	Speed=100;
	CompareR=Speed;
	CompareL=Speed;
	quanshu=1;
	neiquan=0;
	//likai=0;
	LED=1;
	duanming();
	while(1)
	{	
	yikaishi:
		if(P3==0)	//A点线
		{
			CompareL=Speed;
			CompareR=Speed;
			Delay(150);
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
				goto yikaishi;
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
			if(quanshu==2 && (neiquan==0 || neiquan==1))			//内圈
			{
				CompareL=0;
				CompareR=Speed;
				neiquan++;
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
	
	A_Line_Flag=0;
	Speed=100;
	CompareR=Speed;
	CompareL=Speed;
	LED=0;
	likai=0;
	tingguo=0;
	duanming();
	while(1)
	{
		if(A_Line_Flag && tingguo==0)
		{
			A_Line_Flag=0;
			CompareR=0;
			CompareL=0;
			for(i=0;i<15;i++)	//5秒，声光提示
			{
				fmq=0;
				LED=0;
				Delay(100);
				fmq=1;
				LED=1;
				Delay(100);
			}
			Speed=100;
			tingguo=1;
		}
		else if(A_Line_Flag&&tingguo==1)
		{
			A_Line_Flag=0;
			CompareR=Speed;
			CompareL=Speed;
			while(A_Line_On);
			tingguo++;
		}
		else if(A_Line_Flag&&tingguo==2)			//第二次是A点线
		{
			A_Line_Flag=0;
			CompareR=0;
			CompareL=0;
			fengmingqi();
			LED=1;
			shandeng();
			return;
		}
		else
		{
			xunji();
		}
	}
}


void renwu4()			//任务4：超声波避障
{
	A_Line_Flag=0;
	Speed=100;
	CompareR=Speed;
	CompareL=Speed;
	LED=0;
	likai=0;
	chaoshengbo_init();
	duanming();
	while(1)
	{
		if((P3_3==0 || P3_4==0)&&(P3_7==0||P3_6==0)&&P3_0==1)	//分叉
		{
			CompareR=Speed;
			CompareL=Speed;
			Delay(150);	//直走时间
		}
		else if((P3_0==0 ||P3_1==0||P3_2==0)&&(P3_7==1) )
		{
			CompareR=0;		//右转
			CompareL=Speed;
		}
		else if((P3_5==0 ||P3_6==0||P3_7==0)&&(P3_0==1) )
		{
			CompareR=Speed;		//左转
			CompareL=0;
		}

		else
		{
			CompareL=Speed;
			CompareR=Speed;
		}



		
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
		
		if(A_Line_Flag)		//A点线，跑完一圈
		{
			A_Line_Flag=0;
			if(likai==1)
			{
				CompareR=0;
				CompareL=0;
				fengmingqi();
				LED=0;
				while(1);
			}
		}
		else if(A_Line_On==0)
		{
			likai=1;
		}
	}
}
