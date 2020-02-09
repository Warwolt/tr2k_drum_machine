/*
****************************************************************************************************
* brief : this file allows user defined functions to be attached to interrupt signals by calling
*         the user defined function from the associated interrupt service routine.
****************************************************************************************************
*/

#include "Interrupts.h"

static InterruptHandler timer0CompareHandler;
static InterruptHandler timer1CompareHandler;
static InterruptHandler serialTransferHandler;

/**
 * @brief Interrupt service routine for timer1 compare match interrupt.
 */
ISR(TIMER0_COMPA_vect)
{
	timer0CompareHandler();
}

/**
 * @brief Interrupt service routine for timer1 compare match interrupt.
 */
ISR(TIMER1_COMPA_vect)
{
	timer1CompareHandler();
}

/**
 * @brief Interrupt service routine for spi serial transfer complete interrupt.
 */
ISR(SPI_STC_vect)
{
	serialTransferHandler();
}

/**
 * @brief Enable interrupts
 *
 * Enables interrupts signals to trigger service routines to be called. Is a
 * prerequisite for any interrupt handling to enable interrupts globally.
 */
void Interrupts::enableInterruptsGlobally()
{
	sei();
}

/**
 * @brief Disable interrupts
 *
 * Disables interrupts globally, so that no interrupt service routines are
 * called in response to interrupt signals.
 */
void Interrupts::disableInterruptsGlobally()
{
	cli();
}

/**
 * @brief Set an interrupt handler
 *
 * Sets which user defined interrupt handler to call in response to an
 * interrupt signal.
 *
 * @param handler  Pointer to function that handles the interrupt
 * @param request  Enum specifying which signal to set handler for
 */
void Interrupts::setHandlerForInterrupt(const InterruptHandler& handler, InterruptRequest request)
{
	switch (request)
	{
		case(InterruptRequest::Timer0CompareMatch)  : timer0CompareHandler   = handler; break;
		case(InterruptRequest::Timer1CompareMatch)  : timer1CompareHandler   = handler; break;
		case(InterruptRequest::SpiTransferComplete) : serialTransferHandler  = handler; break;
	}
}
