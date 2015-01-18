

#include <stdio.h>
#include "system.h"
#include "io.h"
#include "unistd.h"
#include <stdint.h>

#define PWM_REG_MAXCTR 0x8
#define PWM_REG_UPDATEON_Z 0x9
#define PWM_REG_UPDATEON_MAX 0xA
#define PWM_REG_UPDATE 0xF


// deadband timing. All units ns.
// tOn* = turn on delay
// tOff* = turn off delay
// tRise* = Rise time
// tFall* = Fall time

//static const float tOnFET = 20;
//static const float tRiseFET = 70;
//static const float tOffFET = 30;
static const float tFallFET = 40;
//static const float tOnDriver = 160;
//static const float toffDriver = 150;
//static const float tRiseDriver = 100;
static const float tFallDriver = 50;

static const float tDelayMatchDriver = 50;
static const float tSafety = 50;

static const float deadtime_ns = 200;//tFallFET + tFallDriver + tDelayMatchDriver + tSafety;
static const int deadtimeint = 10;//50000000.0f * deadtime_ns * 1e-9f;

static const int PWMHalfPeriod = 50000000/16000;

// Magnitude must not be larger than sqrt(3)/2, or 0.866
void SVM(float alpha, float beta, uint32_t* tAout, uint32_t* tBout, uint32_t* tCout){

	static const float one_by_sqrt3 = 0.57735026919f;
	static const float two_by_sqrt3 = 1.15470053838f;

	uint32_t Sextant;

	if (beta >= 0.0f)
	{
		if (alpha >= 0.0f)
		{
			//quadrant I
			if (one_by_sqrt3 * beta > alpha)
				Sextant = 2;
			else
				Sextant = 1;

		} else {
			//quadrant II
			if (-one_by_sqrt3 * beta > alpha)
				Sextant = 3;
			else
				Sextant = 2;
		}
	} else {
		if (alpha >= 0.0f)
		{
			//quadrant IV
			if (-one_by_sqrt3 * beta > alpha)
				Sextant = 5;
			else
				Sextant = 6;
		} else {
			//quadrant III
			if (one_by_sqrt3 * beta > alpha)
				Sextant = 4;
			else
				Sextant = 5;
		}
	}

	// PWM timings
	uint32_t tA, tB, tC;

	switch (Sextant) {

		// sextant 1-2
		case 1:
		{
			// Vector on-times
			uint32_t t1 = (alpha - one_by_sqrt3 * beta) * PWMHalfPeriod;
			uint32_t t2 = (two_by_sqrt3 * beta) * PWMHalfPeriod;

			// PWM timings
			tA = (PWMHalfPeriod - t1 - t2) / 2;
			tB = tA + t1;
			tC = tB + t2;

			break;
		}

		// sextant 2-3
		case 2:
		{
			// Vector on-times
			uint32_t t2 = (alpha + one_by_sqrt3 * beta) * PWMHalfPeriod;
			uint32_t t3 = (-alpha + one_by_sqrt3 * beta) * PWMHalfPeriod;

			// PWM timings
			tB = (PWMHalfPeriod - t2 - t3) / 2;
			tA = tB + t3;
			tC = tA + t2;

			break;
		}

		// sextant 3-4
		case 3:
		{
			// Vector on-times
			uint32_t t3 = (two_by_sqrt3 * beta) * PWMHalfPeriod;
			uint32_t t4 = (-alpha - one_by_sqrt3 * beta) * PWMHalfPeriod;

			// PWM timings
			tB = (PWMHalfPeriod - t3 - t4) / 2;
			tC = tB + t3;
			tA = tC + t4;

			break;
		}

		// sextant 4-5
		case 4:
		{
			// Vector on-times
			uint32_t t4 = (-alpha + one_by_sqrt3 * beta) * PWMHalfPeriod;
			uint32_t t5 = (-two_by_sqrt3 * beta) * PWMHalfPeriod;

			// PWM timings
			tC = (PWMHalfPeriod - t4 - t5) / 2;
			tB = tC + t5;
			tA = tB + t4;

			break;
		}

		// sextant 5-6
		case 5:
		{
			// Vector on-times
			uint32_t t5 = (-alpha - one_by_sqrt3 * beta) * PWMHalfPeriod;
			uint32_t t6 = (alpha - one_by_sqrt3 * beta) * PWMHalfPeriod;

			// PWM timings
			tC = (PWMHalfPeriod - t5 - t6) / 2;
			tA = tC + t5;
			tB = tA + t6;

			break;
		}

		// sextant 6-1
		case 6:
		{
			// Vector on-times
			uint32_t t6 = (-two_by_sqrt3 * beta) * PWMHalfPeriod;
			uint32_t t1 = (alpha + one_by_sqrt3 * beta) * PWMHalfPeriod;

			// PWM timings
			tA = (PWMHalfPeriod - t6 - t1) / 2;
			tC = tA + t1;
			tB = tC + t6;

			break;
		}

	} //switch

	*tAout = tA;
	*tBout = tB;
	*tCout = tC;

}

int main()
{
	//printf("Hello from Nios II!\n");

	IOWR(PWM_0_BASE, PWM_REG_UPDATEON_Z, 1);
	IOWR(PWM_0_BASE, PWM_REG_MAXCTR, PWMHalfPeriod);

	for (int ud = 0; 1; ud ^= 1)
	{
		uint32_t tABC[3];
		SVM(ud*0.05,0,&tABC[0],&tABC[1],&tABC[2]);

		for (int i = 0; i < 3; ++i)
		{
			IOWR(PWM_0_BASE, 2*i, tABC[i] - deadtimeint/2);
			IOWR(PWM_0_BASE, 2*i + 1, tABC[i] + deadtimeint/2);
		}

		IOWR(PWM_0_BASE, PWM_REG_UPDATE, 1);

		usleep(500000);
	}
}
