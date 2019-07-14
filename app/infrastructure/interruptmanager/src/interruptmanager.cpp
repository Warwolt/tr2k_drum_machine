/*
****************************************************************************************************
* brief : InterruptManager allows user defined functions to be attached to interrupt signals
*         by calling the user defined function from the associated interrupt service routine.
****************************************************************************************************
*/

#include "interruptmanager.h"

static InterruptHandler timerCompareHandler = nullptr;
static InterruptHandler serialTransferHandler = nullptr;

/**
 *  Interrupt service routine for timer1 compare match interrupt
 */
ISR(TIMER1_COMPA_vect)
{
	if(timerCompareHandler != nullptr)
	{
		timerCompareHandler();
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
void InterruptManager::enableInterruptsGlobally()
{
	sei();
}

/**
 * Disables interrupts globally, so that no interrupt service routines are
 * called in response to interrupt signals.
 */
void InterruptManager::disableInterruptsGlobally()
{
	cli();
}

/**
 * Sets which user defined interrupt handler to call in response to an
 * interrupt signal.
 * @param request  enum specifying which signal to set handler for.
 * @param handler  pointer to function that handles the interrupt.
 */
void InterruptManager::setHandler(InterruptRequest request, InterruptHandler handler)
{
	switch(request)
	{
		case(InterruptRequest::TIMER1_COMPA) : timerCompareHandler   = handler; break;
		case(InterruptRequest::SPI_STC)      : serialTransferHandler = handler; break;
	}
}
