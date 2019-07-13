/*
****************************************************************************************************
* brief : InterruptManager allows user defined functions to be attached to interrupt signals
*         by calling the user defined function from the associated interrupt service routine.
****************************************************************************************************
*/

#include "interruptmanager.h"

static InterruptHandler timerCompareHandler = nullptr;
static InterruptHandler serialTransferHandler = nullptr;

ISR(TIMER1_COMPA_vect)
{
	if(timerCompareHandler != nullptr)
	{
		timerCompareHandler();
	}
}

ISR(SPI_STC_vect)
{
	if(serialTransferHandler != nullptr)
	{
		serialTransferHandler();
	}
}

void InterruptManager::enableInterruptsGlobally()
{
	sei();
}

void InterruptManager::disableInterruptsGlobally()
{
	cli();
}

void InterruptManager::setHandler(InterruptRequest request, InterruptHandler handler)
{
	switch(request)
	{
		case(InterruptRequest::TIMER1_COMPA) : timerCompareHandler   = handler; break;
		case(InterruptRequest::SPI_STC)      : serialTransferHandler = handler; break;
	}
}
