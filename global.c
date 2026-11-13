#include "global.h"

unsigned char Counter,CompareR,CompareL;	//PWM计数值和左右比较值
unsigned char KeyNum,Speed;			//按键号、速度
int quanshu=1;					//圈数
unsigned char juli;				//超声波距离，cm
bit likai;					//已离开A点
int tingguo;					//已等停过
volatile unsigned char A_Line_Flag,A_Line_On;
