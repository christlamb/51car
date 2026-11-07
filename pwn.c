#include <REGX52.H>
#include "Delay.h"
#include "Key.h"
#include "Timer0.h"

void pwm()
{
    while(1)
	{
		KeyNum=Key();
		if(KeyNum==1)
		{
            Compare=50;
		}
        else if(KeyNum == 2)
        {
            Compare=100;
        }
	}
}