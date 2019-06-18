#include "avr/interrupt.h"

static bool hasBeenCalledStatus;

void sei() 
{
	hasBeenCalledStatus = true;
}

void seiResetSpy()
{
	hasBeenCalledStatus = false;
}

bool seiHasBeenCalled()
{
	return hasBeenCalledStatus;
}