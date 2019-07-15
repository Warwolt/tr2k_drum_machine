/*
****************************************************************************************************
* brief : InterruptManager allows user defined functions to be attached to interrupt signals
*         by calling the user defined function from the associated interrupt service routine.
****************************************************************************************************
*/

#ifndef INTERRUPT_MANAGER_H
#define INTERRUPT_MANAGER_H

#include <avr/interrupt.h>

enum class InterruptRequest {TIMER1_COMPA, SPI_STC};
using InterruptHandler = void(*)();

namespace interruptmanager
{

void enableInterruptsGlobally();
void disableInterruptsGlobally();
void setHandlerForInterrupt(InterruptHandler handler, InterruptRequest request);

};

#endif /* INTERRUPT_MANAGER_H */