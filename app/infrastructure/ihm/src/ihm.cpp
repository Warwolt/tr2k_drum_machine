/*
****************************************************************************************************
* brief : InterruptHandlerManager allows user defined functions to be attached to interrupt signals
*         by calling the user defined function from the associated interrupt service routine.
****************************************************************************************************
*/

#include "ihm.h"

static InterruptHandler timerCompareHandler = nullptr;

ISR(TIMER1_COMPA_vect)
{
	if(timerCompareHandler != nullptr)
	{
		timerCompareHandler();
	}
}

void InterruptHandlerManager::enableInterruptsGlobally()
{
	sei();
}

void InterruptHandlerManager::disableInterruptsGlobally()
{
	cli();
}

void InterruptHandlerManager::attachHandler(InterruptHandler handler, InterruptRequest request)
{
	// todo: add test that drives InterruptRequest being used
 	timerCompareHandler = handler;
}
