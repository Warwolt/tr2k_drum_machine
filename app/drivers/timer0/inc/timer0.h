/*
****************************************************************************************************
* brief : Driver for the timer0 8-bit timer peripheral
****************************************************************************************************
*/

#ifndef TIMER0_H
#define TIMER0_H

#include "linuxtypes.h"
#include <avr/io.h>
#include "timer8bit.h"

class Timer0 : public Timer8Bit
{
public:
	Timer0();
	Timer0(u8 &controlRegA, u8 &controlRegB, u8 &interruptMaskReg,
			u8 &outputCompareReg, u8 &counterValueReg);

	void enablePeriodicInterrupts();
	void setPrescaler(PrescaleOption prescaler);
	void setPeriod(u8 period);

	void start();
	void stop();
	void clear();

private:
	volatile u8* controlRegisterA;
	volatile u8* controlRegisterB;
	volatile u8* interruptMaskRegister;
	volatile u8* outputCompareRegister;
	volatile u8* counterValueRegister;
	PrescaleOption currentPrescaler;

	inline void useClearTimerOnCompareMode();
	inline void enableCompareMatchInterrupts();
	inline u8 getPrescalerMask(PrescaleOption prescaler);
	inline void writePrescalerMaskToRegister(u8 mask, volatile u8* reg);
};

#endif /* TIMER0_H */
