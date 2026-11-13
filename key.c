#include "global.h"

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
