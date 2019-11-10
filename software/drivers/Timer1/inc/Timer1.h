/*
****************************************************************************************************
* brief : Driver for the timer1 16-bit timer peripheral
****************************************************************************************************
*/

#ifndef TIMER1_H
#define TIMER1_H

#include "linuxtypes.h"
#include <avr/io.h>
#include "Timer16Bit.h"

class Timer1 : public Timer16Bit
{
public:
	Timer1();
	Timer1(u8 &controlReg, u8 &interruptMaskReg, u16 &outputCompareReg, u16 &counterValueReg);

	void enablePeriodicInterrupts();
	void setPrescaler(PrescaleOption prescaler);
	void setPeriod(u16 period);

	void start();
	void stop();
	void clear();

private:
	volatile u8* controlRegister;
	volatile u8* interruptMaskRegister;
	volatile u16* outputCompareRegister;
	volatile u16* counterValueRegister;
	PrescaleOption currentPrescaler;

	inline void useClearTimerOnCompareMode();
	inline void enableCompareMatchInterrupts();
	inline u8 getPrescalerMask(PrescaleOption prescaler);
	inline void writePrescalerMaskToRegister(u8 mask, volatile u8* reg);
};

#endif /* TIMER1_H */
