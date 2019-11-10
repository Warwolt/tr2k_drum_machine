/*
****************************************************************************************************
* brief : this file allows user defined functions to be attached to interrupt signals by calling
*         the user defined function from the associated interrupt service routine.
****************************************************************************************************
*/

#include "Interrupts.h"

static InterruptHandler timer0CompareHandler = nullptr;
static InterruptHandler timer1CompareHandler = nullptr;
static InterruptHandler serialTransferHandler = nullptr;

/**
 *  Interrupt service routine for timer1 compare match interrupt.
 */
ISR(TIMER0_COMPA_vect)
{
	if(timer0CompareHandler != nullptr)
	{
		timer0CompareHandler();
	}
}

/**
 *  Interrupt service routine for timer1 compare match interrupt.
 */
ISR(TIMER1_COMPA_vect)
{
	if(timer1CompareHandler != nullptr)
	{
		timer1CompareHandler();
	}
}

/**
 * Interrupt service routine for spi serial transfer complete interrupt.
 */
ISR(SPI_STC_vect)
{
	if(serialTransferHandler != nullptr)
	{
		serialTransferHandler();
	}
}

/**
 * Enables interrupts signals to trigger service routines to be called. Is a
 * prerequisite for any interrupt handling to enable interrupts globally.
 */
void Interrupts::enableInterruptsGlobally()
{
	sei();
}

/**
 * Disables interrupts globally, so that no interrupt service routines are
 * called in response to interrupt signals.
 */
void Interrupts::disableInterruptsGlobally()
{
	cli();
}

/**
 * Sets which user defined interrupt handler to call in response to an
 * interrupt signal.
 * @param handler  pointer to function that handles the interrupt.
 * @param request  enum specifying which signal to set handler for.
 */
void Interrupts::setHandlerForInterrupt(InterruptHandler handler, InterruptRequest request)
{
	switch(request)
	{
		case(InterruptRequest::Timer0CompareMatch)  : timer0CompareHandler   = handler; break;
		case(InterruptRequest::Timer1CompareMatch)  : timer1CompareHandler   = handler; break;
		case(InterruptRequest::SpiTransferComplete) : serialTransferHandler  = handler; break;
	}
}
