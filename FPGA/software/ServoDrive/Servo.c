

#include <stdio.h>
#include "system.h"
#include "io.h"
#include "unistd.h"
#include <stdint.h>
#include "extramath/extramath.h"


#define PWM_REG_MAXCTR 0x8
#define PWM_REG_EN 0x9
#define PWM_REG_UPDATEON_Z 0xA
#define PWM_REG_UPDATEON_MAX 0xB
#define PWM_REG_TRIGON_Z 0xC
#define PWM_REG_TRIGON_MAX 0xD
#define PWM_REG_UPDATE 0xF

#define TADCS_REG_EN 0
#define TADCS_REG_IRQFLAG 1
#define TADCS_REG_MAXSEQ 2
#define TADCS_OFFSET_CH_MAP 0x10
#define TADCS_OFFSET_SAMPSTORE 0x18

#define QEI_REG_COUNT 0x0
#define QEI_REG_clearOnZ 0x1
#define QEI_REG_revDir 0x2

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

//static const float deadtime_ns = 200;//tFallFET + tFallDriver + tDelayMatchDriver + tSafety;
#define deadtime_ns 200
static const int deadtimeint = (float)ALT_CPU_CPU_FREQ * deadtime_ns * 1e-9f;

#define PWMFrequency 16000
static const float PWMPeriod = 1.0f/(float)PWMFrequency;

static const int PWMHalfPeriod = ALT_CPU_CPU_FREQ/PWMFrequency;

//#define QudcountsPerRev 2000
#define QudcountsPerRev 2400

static const float ADCtoAscalefactor = 3.3f/((float)(1<<12) * 50.0f * 0.0005f);
static const float encToPhasefactor = 2.0f*PI_F*7.0f/(float)QudcountsPerRev;
static const float omegaFilterConst = 0.98;

static const float lockinCurrent = 10.0f;

//donkey
static const float currentKp = 0.002f;
static const float currentKi = 10.0f;


//350kv motor
//static const float currentKp = 0.004f;
//static const float currentKi = 15.0f;
//static const float speedKp = 0.1f;
//static const float speedKi = 1.0f;
static const float speedKp = 0.02f;
static const float speedKi = 0.0f;
static const float currentlimit = 50.0f;

// Magnitude must not be larger than sqrt(3)/2, or 0.866
void SVM(float alpha, float beta, uint32_t* tAout, uint32_t* tBout, uint32_t* tCout){

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

void control_current(float ItargetAlpha, float ItargetBeta, float Ia, float Ib){

	float Ialpha = Ia;
	float Ibeta = one_by_sqrt3 * Ia + two_by_sqrt3 * Ib; 

	float IerrAlpha = ItargetAlpha - Ialpha;
	float IerrBeta = ItargetBeta - Ibeta;

	static float IerrVstateAlpha = 0.0f;
	static float IerrVstateBeta = 0.0f;

	float Valpha = IerrVstateAlpha + IerrAlpha * currentKp;
	float Vbeta = IerrVstateBeta + IerrBeta * currentKp;

	float Vscalefactor = sqrt3_by_2 * Q_rsqrt(Valpha*Valpha + Vbeta*Vbeta);
	if (Vscalefactor < 1)
	{
		Valpha *= Vscalefactor;
		Vbeta *= Vscalefactor;
	} else {
		IerrVstateAlpha += IerrAlpha * (currentKi * PWMPeriod);
		IerrVstateBeta += IerrBeta * (currentKi * PWMPeriod);
	}

	uint32_t tABC[3];
	SVM(Valpha,Vbeta,&tABC[0],&tABC[1],&tABC[2]);

	for (int i = 0; i < 3; ++i)
	{
		IOWR(PWM_0_BASE, 2*i, MAX((int)tABC[i] - deadtimeint/2, 0));
		IOWR(PWM_0_BASE, 2*i + 1, tABC[i] + deadtimeint/2);
	}

	IOWR(PWM_0_BASE, PWM_REG_UPDATE, 1);
}

static int IsenseOffset[2] = {0};

void blocking_polar_control_current(float phase, float mag){
	while(!IORD(TRIGGERED_ADC_SEQUENCER_0_BASE, TADCS_REG_IRQFLAG));
	IOWR(TRIGGERED_ADC_SEQUENCER_0_BASE, TADCS_REG_IRQFLAG, 0);

	IOWR(TRIGGERED_ADC_SEQUENCER_0_BASE, TADCS_REG_IRQFLAG, 0);
	float Ia = ADCtoAscalefactor * ((int)IORD(TRIGGERED_ADC_SEQUENCER_0_BASE, TADCS_OFFSET_SAMPSTORE + 0) - IsenseOffset[0]);
	float Ib = ADCtoAscalefactor * ((int)IORD(TRIGGERED_ADC_SEQUENCER_0_BASE, TADCS_OFFSET_SAMPSTORE + 1) - IsenseOffset[1]);
	float c,s;
	fast_cossin(phase, &c, &s);

	control_current(mag*c, mag*s, Ia, Ib);
}

int main()
{
	//printf("Hello from Nios II!\n");

	//350kv motor vice rig
	//IOWR(QEI_0_BASE, QEI_REG_revDir, 1); //encoder reversed dir compared to motor

	//Teleport rig
	IOWR(QEI_0_BASE, QEI_REG_revDir, 0);

	IOWR(TRIGGERED_ADC_SEQUENCER_0_BASE, TADCS_REG_MAXSEQ, 1);
	IOWR(TRIGGERED_ADC_SEQUENCER_0_BASE, TADCS_OFFSET_CH_MAP + 0, 7);
	IOWR(TRIGGERED_ADC_SEQUENCER_0_BASE, TADCS_OFFSET_CH_MAP + 1, 6),
	IOWR(TRIGGERED_ADC_SEQUENCER_0_BASE, TADCS_REG_IRQFLAG, 0);
	IOWR(TRIGGERED_ADC_SEQUENCER_0_BASE, TADCS_REG_EN, 1);

	IOWR(PWM_0_BASE, PWM_REG_TRIGON_Z, 1);
	IOWR(PWM_0_BASE, PWM_REG_UPDATEON_Z, 1);
	IOWR(PWM_0_BASE, PWM_REG_MAXCTR, PWMHalfPeriod);
	for (int i = 0; i < 3; ++i)
	{
		IOWR(PWM_0_BASE, 2*i, PWMHalfPeriod/2 - deadtimeint/2);//MAX((int)tABC[i] - deadtimeint/2, 0));
		IOWR(PWM_0_BASE, 2*i + 1, PWMHalfPeriod/2 + deadtimeint/2);
	}
	IOWR(PWM_0_BASE, PWM_REG_UPDATE, 1);
	IOWR(PWM_0_BASE, PWM_REG_EN, 1);

	//sample current sense values during V0 PWM
	for (int i = -128; i < 128; ++i)
	{
		while(!IORD(TRIGGERED_ADC_SEQUENCER_0_BASE, TADCS_REG_IRQFLAG));
		IOWR(TRIGGERED_ADC_SEQUENCER_0_BASE, TADCS_REG_IRQFLAG, 0);
		int Ia = IORD(TRIGGERED_ADC_SEQUENCER_0_BASE, TADCS_OFFSET_SAMPSTORE + 0);
		int Ib = IORD(TRIGGERED_ADC_SEQUENCER_0_BASE, TADCS_OFFSET_SAMPSTORE + 1);
		if(i >= 0){
			IsenseOffset[0] += Ia;
			IsenseOffset[1] += Ib;
		}

		for (int i = 0; i < 3; ++i)
		{
			IOWR(PWM_0_BASE, 2*i, PWMHalfPeriod/2 - deadtimeint/2);//MAX((int)tABC[i] - deadtimeint/2, 0));
			IOWR(PWM_0_BASE, 2*i + 1, PWMHalfPeriod/2 + deadtimeint/2);
		}

		IOWR(PWM_0_BASE, PWM_REG_UPDATE, 1);
	}

	for (int i = 0; i < 2; ++i)
	{
		IsenseOffset[i] /= 128;
	}

	//Fixed voltage test
	while(0){
		uint32_t tABC[3];
		SVM(0.02f,0.0f,&tABC[0],&tABC[1],&tABC[2]);

		for (int i = 0; i < 3; ++i)
		{
			IOWR(PWM_0_BASE, 2*i, MAX((int)tABC[i] - deadtimeint/2, 0));
			IOWR(PWM_0_BASE, 2*i + 1, tABC[i] + deadtimeint/2);
		}

		IOWR(PWM_0_BASE, PWM_REG_UPDATE, 1);
	}

	//square wave current control test
	while(0){
		for (int i = 0; i < 100; ++i)
		{
			blocking_polar_control_current(0.0f, 10.0f);
		}
		for (int i = 0; i < 100; ++i)
		{
			blocking_polar_control_current(0.0f, 0.0f);
		}
	}

#ifdef ENC_IDX_PRESENT

	IOWR(QEI_0_BASE, QEI_REG_clearOnZ, 1); //clear phase reg on next index pulse

	//scan lockin until idx pulse
	for (float ph = 0; IORD(QEI_0_BASE, QEI_REG_clearOnZ); ph += 0.0022f)
	{
		blocking_polar_control_current(ph, lockinCurrent);
	}

#else
	IOWR(QEI_0_BASE, QEI_REG_COUNT, 0);
#endif

	//go to rotor zero phase to get ready to scan
	for (int i = 0; i < (0.5f*PWMFrequency); ++i)
	{
		blocking_polar_control_current(-2*PI_F, lockinCurrent);
	}

	//scan forwards
	int encvaluesum = 0;
	for (float ph = -2*PI_F; ph < 2*PI_F; ph += 4*PI_F/128.0f)
	{
		for (int i = 0; i < (0.01f*PWMFrequency); ++i)
		{
			blocking_polar_control_current(ph, lockinCurrent);
		}
		encvaluesum += IORD(QEI_0_BASE, QEI_REG_COUNT);
	}
	int testencval = IORD(QEI_0_BASE, QEI_REG_COUNT);
	if(testencval < 0/*-(QudcountsPerRev/(7*2))*/) //encoder vs motor phases likely configured backwards
		while(1){
			//TODO flash red phase LED here
		}
	for (float ph = 2*PI_F; ph > -2*PI_F; ph -= 4*PI_F/128.0f)
	{
		for (int i = 0; i < (0.01f*PWMFrequency); ++i)
		{
			blocking_polar_control_current(ph, lockinCurrent);
		}
		encvaluesum += IORD(QEI_0_BASE, QEI_REG_COUNT);
	}

	int encoffset = encvaluesum / 256;

	//while(1);

	int oldenc = IORD(QEI_0_BASE, QEI_REG_COUNT);
	while(1)
	for(int i = -2000; i < 2000; ++i){

		while(!IORD(TRIGGERED_ADC_SEQUENCER_0_BASE, TADCS_REG_IRQFLAG));
		IOWR(TRIGGERED_ADC_SEQUENCER_0_BASE, TADCS_REG_IRQFLAG, 0);

		IOWR(TRIGGERED_ADC_SEQUENCER_0_BASE, TADCS_REG_IRQFLAG, 0);
		float Ia = ADCtoAscalefactor * ((int)IORD(TRIGGERED_ADC_SEQUENCER_0_BASE, TADCS_OFFSET_SAMPSTORE + 0) - IsenseOffset[0]);
		float Ib = ADCtoAscalefactor * ((int)IORD(TRIGGERED_ADC_SEQUENCER_0_BASE, TADCS_OFFSET_SAMPSTORE + 1) - IsenseOffset[1]);

		int enccount = IORD(QEI_0_BASE, QEI_REG_COUNT);
		float phase = ((enccount - encoffset) % QudcountsPerRev) * encToPhasefactor;
		float c,s;
		fast_cossin(phase, &c, &s);

		float dencbydt = encToPhasefactor*(enccount - oldenc)*PWMFrequency;
		oldenc = enccount;

		static float omega = 0.0f;
		omega = omegaFilterConst*omega + (1-omegaFilterConst)*dencbydt;

		static int logctr = 0;
		if(++logctr == 1){
			logctr = 0;
			IOWR(LOG_REG_0_BASE, 0, (int)(omega*10.0f));
		}

		float omegasetpoint = (i<0) ? 2000.0f : -2000.0f;
		omegasetpoint -= enccount * 0.1f;
		float omegaerror = omegasetpoint - omega;

		static float Iqintstate = 0.0f;

		float Id = 0.0f;
		float Iq = speedKp * omegaerror + Iqintstate;

		if(Iq > currentlimit){
			Iq = currentlimit;
		}else{
			if(Iq < -currentlimit){
				Iq = -currentlimit;
			} else {
				Iqintstate += omegaerror * (speedKi * PWMPeriod);
			}
		}

		float Ialpha = c*Id - s*Iq;
		float Ibeta  = c*Iq + s*Id;

		control_current(Ialpha, Ibeta, Ia, Ib);

	}
}

////Use only for simulation!
//int alt_main(){
//	return main();
//}
