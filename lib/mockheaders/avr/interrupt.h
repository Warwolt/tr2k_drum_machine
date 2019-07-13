// partial mock implementation of avr/interrupt.h for use in tests.

#ifndef _AVR_INTERRUPT_H_
#define _AVR_INTERRUPT_H_

#include <avr/io.h>

void sei(); // sei (enable interrupts globally)
void cli(); // cli (disable interrupts globally)

// interrupt service routine
#define ISR(VECTOR) void VECTOR (void)

namespace avrmock
{
bool seiWasCalled();
void resetSeiWasCalled();
bool cliWasCalled();
void resetCliWasCalled();
}

#endif /* _AVR_INTERRUPT_H_ */
