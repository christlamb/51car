#include "global.h"

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
