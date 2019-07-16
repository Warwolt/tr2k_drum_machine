// partial mock implementation of avr/interrupt.h for use in tests.

#include <avr/interrupt.h>

namespace AvrMock
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

} /* AvrMock */


// sei (enable interrupts globally)
void sei()
{
	AvrMock::seiCalled = true;
}

// cli (disable interrupts globally)
void cli()
{
	AvrMock::cliCalled = true;
}
