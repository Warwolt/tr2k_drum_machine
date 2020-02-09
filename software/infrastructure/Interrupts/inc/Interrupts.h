/*
****************************************************************************************************
* brief : This file allows user defined functions to be attached to interrupt signals by calling
*         the user defined function from the associated interrupt service routine.
****************************************************************************************************
*/

#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include <avr/interrupt.h>
#include "r2k/function.h"

enum class InterruptRequest
{
    Timer0CompareMatch,
    Timer1CompareMatch,
    SpiTransferComplete
};
using InterruptHandler = r2k::function<void()>;

namespace Interrupts
{

void enableInterruptsGlobally();
void disableInterruptsGlobally();
void setHandlerForInterrupt(const InterruptHandler &handler, InterruptRequest request);

}; // namespace Interrupts

#endif /* INTERRUPTS_H */
