/*
****************************************************************************************************
* brief : Mock implementation of Timer16Bit interface
****************************************************************************************************
*/

#ifndef MILLISECOND_TIMER_MOCK
#define MILLISECOND_TIMER_MOCK

#include "gmock/gmock.h"
#include "millisecond_timer.h"
#include "linuxtypes.h"

class MillisecondTimerMock : public MillisecondTimer
{
public:
	MOCK_METHOD0(getCurrentTime, milliseconds());
};

#endif /* MILLISECOND_TIMER_MOCK */
