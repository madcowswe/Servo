//*****************************************************************************
//
// dead_band.c - Example demonstrating the dead-band generator.
//
// Copyright (c) 2010-2013 Texas Instruments Incorporated.  All rights reserved.
// Software License Agreement
// 
//   Redistribution and use in source and binary forms, with or without
//   modification, are permitted provided that the following conditions
//   are met:
// 
//   Redistributions of source code must retain the above copyright
//   notice, this list of conditions and the following disclaimer.
// 
//   Redistributions in binary form must reproduce the above copyright
//   notice, this list of conditions and the following disclaimer in the
//   documentation and/or other materials provided with the  
//   distribution.
// 
//   Neither the name of Texas Instruments Incorporated nor the names of
//   its contributors may be used to endorse or promote products derived
//   from this software without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// 
// This is part of revision 2.0.1.11577 of the Tiva Firmware Development Package.
//
//*****************************************************************************

#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/pwm.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"

//*****************************************************************************
//
//! \addtogroup pwm_examples_list
//! <h1>PWM dead-band (dead_band)</h1>
//!
//! This example shows how to setup the PWM0 block with a dead-band generation.
//!
//! This example uses the following peripherals and I/O signals.  You must
//! review these and change as needed for your own board:
//! - GPIO Port B peripheral (for PWM pins)
//! - M0PWM0 - PB6
//! - M0PWM1 - PB7
//!
//! The following UART signals are configured only for displaying console
//! messages for this example.  These are not required for operation of the
//! PWM.
//! - UART0 peripheral
//! - GPIO Port A peripheral (for UART0 pins)
//! - UART0RX - PA0
//! - UART0TX - PA1
//!
//! This example uses the following interrupt handlers.  To use this example
//! in your own application you must add these interrupt handlers to your
//! vector table.
//! - None.
//
//*****************************************************************************


//The serial port speed
#define BAUDRATE 115200

#define MIN(X,Y) ((X) < (Y) ? (X) : (Y))
#define MAX(X,Y) ((X) > (Y) ? (X) : (Y))


// Uses floating point, only use in constant expressions!
#define CLOCKRATE 50000000
#define ClocksToUS(clocks) (((float)clocks / CLOCKRATE)*1000000)
#define USToClocks(us) ((unsigned int)((us/1000000.0f)*CLOCKRATE))
#define SToClocks(s) ((unsigned int)((float)s*CLOCKRATE))
#define FtoClocks(f) ((unsigned int)((float)CLOCKRATE/f))

//*****************************************************************************
//
// The error routine that is called if the driver library encounters an error.
//
//*****************************************************************************
#ifdef DEBUG
void
__error__(char *pcFilename, uint32_t ui32Line)
{
	UARTprintf("Library Error: line %d\n",ui32Line);
}
#endif

//*****************************************************************************
//
// Configure the UART and its pins.  This must be called before UARTprintf().
//
//*****************************************************************************
void
ConfigureUART(void)
{
	//
	// Enable the GPIO Peripheral used by the UART.
	//
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

	//
	// Enable UART0
	//
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);

	//
	// Configure GPIO Pins for UART mode.
	//
	ROM_GPIOPinConfigure(GPIO_PA0_U0RX);
	ROM_GPIOPinConfigure(GPIO_PA1_U0TX);
	ROM_GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

	//
	// Use the internal 16MHz oscillator as the UART clock source.
	//
	UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);

	//
	// Initialize the UART for console I/O.
	//
	UARTStdioConfig(0, BAUDRATE, 16000000);
}


//*****************************************************************************
//
// Configure the PWM0 block with dead-band generation.  The example configures
// the PWM0 block to generate a 25% duty cycle signal on PD0 with dead-band
// generation.  This will produce a complement of PD0 on PD1 (75% duty cycle).
// The dead-band generator is set to have a 10us or 160 cycle delay
// (160cycles / 16Mhz = 10us) on the rising and falling edges of the PD0 PWM
// signal.
//
//*****************************************************************************
int
main(void)
{


	//
	// Enable lazy stacking for interrupt handlers.  This allows floating-point
	// instructions to be used within interrupt handlers, but at the expense of
	// extra stack usage.
	//
	//MAP_FPULazyStackingEnable();
	MAP_FPUStackingDisable(); // we are not using floating point in ISR's.

	IntMasterDisable(); //Turn off interrupts while we configure system.


	MAP_SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ |
					   SYSCTL_OSC_MAIN);

	// Set the PWM clock to the system clock.
	MAP_SysCtlPWMClockSet(SYSCTL_PWMDIV_1);


	// Turn on all used peripherals
	MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOH);
	MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOG);
	MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);
	MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
	// wait for units to power on
	SysCtlDelay(10);

	//
	// Initialize the UART.
	//
	ConfigureUART();

	// Configure the GPIO pin muxing to select PWM functions for these pins.
	MAP_GPIOPinConfigure(GPIO_PH0_M0PWM0);
	MAP_GPIOPinConfigure(GPIO_PH1_M0PWM1);
	MAP_GPIOPinConfigure(GPIO_PH2_M0PWM2);
	MAP_GPIOPinConfigure(GPIO_PH3_M0PWM3);
	MAP_GPIOPinConfigure(GPIO_PG6_M0PWM6); //note skip of 4 and 5!
	MAP_GPIOPinConfigure(GPIO_PG7_M0PWM7);

	// Configure the GPIO pad for PWM function
	MAP_GPIOPinTypePWM(GPIO_PORTH_BASE, GPIO_PIN_0);
	MAP_GPIOPinTypePWM(GPIO_PORTH_BASE, GPIO_PIN_1);
	MAP_GPIOPinTypePWM(GPIO_PORTH_BASE, GPIO_PIN_2);
	MAP_GPIOPinTypePWM(GPIO_PORTH_BASE, GPIO_PIN_3);
	MAP_GPIOPinTypePWM(GPIO_PORTG_BASE, GPIO_PIN_6); //note skip of 4 and 5!
	MAP_GPIOPinTypePWM(GPIO_PORTG_BASE, GPIO_PIN_7);

	//override above to set 8ma drive strength (since we are using long wires)
	MAP_GPIOPadConfigSet(GPIO_PORTH_BASE, GPIO_PIN_0, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD);
	MAP_GPIOPadConfigSet(GPIO_PORTH_BASE, GPIO_PIN_1, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD);
	MAP_GPIOPadConfigSet(GPIO_PORTH_BASE, GPIO_PIN_2, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD);
	MAP_GPIOPadConfigSet(GPIO_PORTH_BASE, GPIO_PIN_3, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD);
	MAP_GPIOPadConfigSet(GPIO_PORTG_BASE, GPIO_PIN_6, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD);
	MAP_GPIOPadConfigSet(GPIO_PORTG_BASE, GPIO_PIN_7, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD);

	uint32_t PWMPeriod = FtoClocks(8000);

	// Configure the PWM
	uint32_t PWMGENS[] = { PWM_GEN_0, PWM_GEN_1, PWM_GEN_3}; //note skip of block 2!
	for (int i = 0; i < 3; ++i)
	{
		PWMGenConfigure(PWM0_BASE, PWMGENS[i],
		PWM_GEN_MODE_UP_DOWN
		| PWM_GEN_MODE_NO_SYNC //Local sync of CMP and load
		| PWM_GEN_MODE_DBG_RUN
		| PWM_GEN_MODE_GEN_NO_SYNC //No change when running
		| PWM_GEN_MODE_DB_NO_SYNC //No change when running
		| PWM_GEN_MODE_FAULT_LATCHED
		| PWM_GEN_MODE_FAULT_EXT
		);

		// Set the PWM period to 8kHz
		PWMGenPeriodSet(PWM0_BASE, PWMGENS[i], PWMPeriod);
	}

	// Set to 50% pulse width by default
	PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0, PWMPeriod / 2); 
	PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1, PWMPeriod / 2);
	PWMPulseWidthSet(PWM0_BASE, PWM_OUT_3, PWMPeriod / 2); //note skip of block 2!

	// deadband timing. All units ns.
	// tOn* = turn on delay
	// tOff* = turn off delay
	// tRise* = Rise time
	// tFall* = Fall time
	const float tOnFET = 20;
	const float tRiseFET = 70;
	const float tOffFET = 30;
	const float tFallFET = 40;
	const float tOnDriver = 160;
	const float toffDriver = 150;
	const float tRiseDriver = 100;
	const float tFallDriver = 50;

	const float tDelayMatchDriver = 50;
	const float tSafety = 50;

	const float asdf = tRiseFET + tRiseDriver;

	//
	// Enable the dead-band generation on the PWM0 output signal.  PWM bit 0
	// (PD0), will have a duty cycle of 25% (set above) and PWM bit 1 will have
	// a duty cycle of 75%.  These signals will have a 10us gap between the
	// rising and falling edges.  This means that before PWM bit 1 goes high,
	// PWM bit 0 has been low for at LEAST 160 cycles (or 10us) and the same
	// before PWM bit 0 goes high.  The dead-band generator lets you specify
	// the width of the "dead-band" delay, in PWM clock cycles, before the PWM
	// signal goes high and after the PWM signal falls.  For this example we
	// will use 160 cycles (or 10us) on both the rising and falling edges of
	// PD0.  Reference the datasheet for more information on dead-band
	// generation.
	//
	PWMDeadBandEnable(PWM0_BASE, PWM_GEN_0, USToClocks(, USToClocks);

	//
	// Enable the PWM0 Bit 0 (PD0) and Bit 1 (PD1) output signals.
	//
	PWMOutputState(PWM0_BASE, PWM_OUT_1_BIT | PWM_OUT_0_BIT, true);

	//
	// Enables the counter for a PWM generator block.
	//
	PWMGenEnable(PWM0_BASE, PWM_GEN_0);

	//
	// Loop forever while the PWM signals are generated.
	//
	while(1)
	{
		//
		// Print out indication on the console that the program is running.
		//
		PrintRunningDots();
	}
}
