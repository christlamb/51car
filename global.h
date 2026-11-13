#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include <REGX52.H>

sbit MotorR=P1^2;	//右电机
sbit MotorL=P1^3;	//左电机
sbit fmq=P1^0;		//蜂鸣器
sbit LED=P1^1;		//LED
sbit Trig=P2^0;		//超声波Trig
sbit Echo=P2^1;		//超声波Echo

extern unsigned char Counter,CompareR,CompareL;
extern unsigned char KeyNum,Speed;
extern int quanshu;
extern unsigned char juli;
extern bit likai;
extern int tingguo;
extern volatile unsigned char A_Line_Flag,A_Line_On;

void Delay(unsigned int xms);
unsigned char Key();
void Timer0_Init();
void fengmingqi();
void duanming();
void shandeng();
void xunji();
void chaoshengbo_init();
unsigned char chaoshengbo();
void renwu1();
void renwu2();
void renwu3();
void renwu4();
void tiaosu();

#endif
