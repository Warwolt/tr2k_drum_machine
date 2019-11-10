/*
****************************************************************************************************
* brief : Driver for the timer1 16-bit timer peripheral
****************************************************************************************************
*/

#include "Timer1.h"

/**
 * Default constructor, initializes register pointers.
 */
Timer1::Timer1() : controlRegister(&TCCR1B), interruptMaskRegister(&TIMSK1),
	outputCompareRegister(&OCR1A), counterValueRegister(&TCNT1)
{

}

/**
 * Constructor used in tests.
 */
Timer1::Timer1(u8 &controlReg, u8 &interruptMaskReg, u16 &outputCompareReg, u16 &counterValueReg) :
	controlRegister(&controlReg), interruptMaskRegister(&interruptMaskReg),
	outputCompareRegister(&outputCompareReg), counterValueRegister(&counterValueReg)
{

}

/**
 * Enables the timer to generate TIMER1_COMPA interrupts at a frequency
 * determined by the last argument given to Timer1::setPeriod().
 */
void Timer1::enablePeriodicInterrupts()
{
	useClearTimerOnCompareMode();
	enableCompareMatchInterrupts();
}

inline void Timer1::useClearTimerOnCompareMode()
{
	*controlRegister |= 0x1 << WGM12;
}

inline void Timer1::enableCompareMatchInterrupts()
{
	*interruptMaskRegister |= 0x1 << OCIE1A;
}

/**
 * Set which prescaler to divide clock source by.
 * @param prescaler  enum containing legal prescale-values.
 */
void Timer1::setPrescaler(PrescaleOption prescaler)
{
	currentPrescaler = prescaler;
}

/**
 * Set the period for the timer before counter rolls back to to 0.
 * If interrupts have been enabled, this period will be the time
 * between each TIMER1_COMPA interrupt.
 */
void Timer1::setPeriod(u16 period)
{
	*outputCompareRegister = period - 1;
}

/**
 * Starts the timer by selecting a prescaled clock source.
 */
void Timer1::start()
{
	u8 mask = getPrescalerMask(currentPrescaler);
	writePrescalerMaskToRegister(mask, controlRegister);
}

inline u8 Timer1::getPrescalerMask(PrescaleOption prescaler)
{
	switch (prescaler)
	{
		case PrescaleOption::_1    : return (0x1 << CS10);
		case PrescaleOption::_8    : return (0x2 << CS10);
		case PrescaleOption::_64   : return (0x3 << CS10);
		case PrescaleOption::_256  : return (0x4 << CS10);
		case PrescaleOption::_1024 : return (0x5 << CS10);
		default : return 0;
	}
}

inline void Timer1::writePrescalerMaskToRegister(u8 mask, volatile u8* reg)
{
	constexpr u8 widthmask = 0x7; // clock source selected with 3 bits
	*reg &= mask | ~widthmask; // write zeros in mask
	*reg |= mask & widthmask; // write ones in mask
}

/**
 * Stops the timer by setting the clock source to none.
 */
void Timer1::stop()
{
	constexpr u8 widthmask = 0x7; // clock source selected with 3 bits
	*controlRegister &= ~widthmask;
}

/**
 * Clears the timer counter register so that the current period is restarted.
 */
void Timer1::clear()
{
	*counterValueRegister = 0;
}
