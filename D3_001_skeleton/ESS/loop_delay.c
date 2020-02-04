
#include "loop_delay.h"

void delay_msec(uint32_t delay)
{
	// internal loop counter
	uint32_t k;
	// dummy variable to prevent compiler optimization
	volatile uint32_t internal_fake; 
	while (delay-- >0)
	{
		for (k = 0; k <20000;k++)
		{
			internal_fake--;
		}
	}
}

void delay_usec(uint32_t delay)
{
	// internal loop counter
	uint32_t k;
	// dummy variable to prevent compiler optimization
	volatile uint32_t internal_fake; 
	while (delay-- >0)
	{
		for (k = 0; k <20;k++)
		{
			internal_fake--;
		}
	}
}

