// partial mock implementation of avr/interrupt.h for use in tests.

#include <avr/interrupt.h>

namespace avrmock
{

static bool seiCalled = false;
static bool cliCalled = false;

bool seiWasCalled()
{
	return seiCalled;
}

void resetSeiWasCalled()
{
	seiCalled = false;
}

bool cliWasCalled()
{
	return cliCalled;
}

void resetCliWasCalled()
{
	cliCalled = false;
}

} // avrmock

// sei (enable interrupts globally)
void sei()
{
	avrmock::seiCalled = true;
}

// cli (disable interrupts globally)
void cli()
{
	avrmock::cliCalled = true;
}