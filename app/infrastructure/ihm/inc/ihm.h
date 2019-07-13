/*
****************************************************************************************************
* brief : InterruptHandlerManager allows user defined functions to be attached to interrupt signals
*         by calling the user defined function from the associated interrupt service routine.
****************************************************************************************************
*/

#ifndef INTERRUPT_HANDLER_MANAGER_H
#define INTERRUPT_HANDLER_MANAGER_H

#include <avr/interrupt.h>

enum class InterruptRequest {TIMER1_COMPA};
using InterruptHandler = void(*)();

class InterruptHandlerManager
{
public:
	void enableInterruptsGlobally();
	void disableInterruptsGlobally();
	void attachHandler(InterruptHandler handler, InterruptRequest request);
};

#endif /* INTERRUPT_HANDLER_MANAGER_H */
