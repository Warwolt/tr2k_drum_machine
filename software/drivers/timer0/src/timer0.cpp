/*
****************************************************************************************************
* brief : Driver for the timer0 16-bit timer peripheral
****************************************************************************************************
*/

#include "timer0.h"

/**
 * Default constructor, initializes register pointers.
 */
Timer0::Timer0() : controlRegisterA(&TCCR0A), controlRegisterB(&TCCR0B),
	interruptMaskRegister(&TIMSK0),	outputCompareRegister(&OCR0A), counterValueRegister(&TCNT0)
{

}

/**
 * Constructor used in tests.
 */
Timer0::Timer0(u8 &controlRegA, u8 &controlRegB, u8 &interruptMaskReg, u8 &outputCompareReg,
	u8 &counterValueReg) : controlRegisterA(&controlRegA), controlRegisterB(&controlRegB),
	interruptMaskRegister(&interruptMaskReg), outputCompareRegister(&outputCompareReg),
	counterValueRegister(&counterValueReg)
{

}

/**
 * Enables the timer to generate timer0_COMPA interrupts at a frequency
 * determined by the last argument given to Timer0::setPeriod().
 */
void Timer0::enablePeriodicInterrupts()
{
	useClearTimerOnCompareMode();
	enableCompareMatchInterrupts();
}

inline void Timer0::useClearTimerOnCompareMode()
{
	*controlRegisterA |= 0x1 << WGM01;
}

inline void Timer0::enableCompareMatchInterrupts()
{
	*interruptMaskRegister |= 0x1 << OCIE0A;
}

/**
 * Set which prescaler to divide clock source by.
 * @param prescaler  enum containing legal prescale-values.
 */
void Timer0::setPrescaler(PrescaleOption prescaler)
{
	currentPrescaler = prescaler;
}

/**
 * Set the period for the timer before counter rolls back to to 0.
 * If interrupts have been enabled, this period will be the time
 * between each timer0_COMPA interrupt.
 */
void Timer0::setPeriod(u8 period)
{
	*outputCompareRegister = period - 1;
}

/**
 * Starts the timer by selecting a prescaled clock source.
 */
void Timer0::start()
{
	u8 mask = getPrescalerMask(currentPrescaler);
	writePrescalerMaskToRegister(mask, controlRegisterB);
}

inline u8 Timer0::getPrescalerMask(PrescaleOption prescaler)
{
	switch (prescaler)
	{
		case PrescaleOption::_1    : return (0x1 << CS00);
		case PrescaleOption::_8    : return (0x2 << CS00);
		case PrescaleOption::_64   : return (0x3 << CS00);
		case PrescaleOption::_256  : return (0x4 << CS00);
		case PrescaleOption::_1024 : return (0x5 << CS00);
		default : return 0;
	}
}

inline void Timer0::writePrescalerMaskToRegister(u8 mask, volatile u8* reg)
{
	constexpr u8 widthmask = 0x7; // clock source selected with 3 bits
	*reg &= mask | ~widthmask; // write zeros in mask
	*reg |= mask & widthmask; // write ones in mask
}

/**
 * Stops the timer by setting the clock source to none.
 */
void Timer0::stop()
{
	constexpr u8 widthmask = 0x7; // clock source selected with 3 bits
	*controlRegisterB &= ~widthmask;
}

/**
 * Clears the timer counter register so that the current period is restarted.
 */
void Timer0::clear()
{
	*counterValueRegister = 0;
}
