

#include <stdio.h>
#include "system.h"
#include "io.h"
#include "unistd.h"

#define PWM_REG_MAXCTR 0x8
#define PWM_REG_UPDATEON_Z 0x9
#define PWM_REG_UPDATEON_MAX 0xA
#define PWM_REG_UPDATE 0xF

int main()
{
	//printf("Hello from Nios II!\n");

	int halfperiod = 50000000/16000;

	IOWR(PWM_0_BASE, PWM_REG_UPDATEON_Z, 1);
	IOWR(PWM_0_BASE, PWM_REG_MAXCTR, halfperiod);

	for (int ud = 0; 1; ud ^= 1)
	{
		for (int i = 0; i < 6; ++i)
		{
			IOWR(PWM_0_BASE, i, (halfperiod+(ud*1000))/2);
		}

		IOWR(PWM_0_BASE, PWM_REG_UPDATE, 1);

		usleep(500000);
	}
}
