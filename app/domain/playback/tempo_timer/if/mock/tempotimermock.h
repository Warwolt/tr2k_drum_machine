/*
****************************************************************************************************
* brief : Mock implementation of Timer16Bit interface
****************************************************************************************************
*/

#ifndef TIMER_16_BIT_MOCK_H
#define TIMER_16_BIT_MOCK_H

#include "gmock/gmock.h"
#include "tempotimer.h"
#include "linuxtypes.h"

class TempoTimerMock : public TempoTimer
{
public:
	MOCK_METHOD1(setTempo, void(BeatsPerMinute bpm));
	MOCK_METHOD0(start, void());
	MOCK_METHOD0(stop, void());
	MOCK_METHOD0(clear, void());
	MOCK_METHOD0(playbackStepIsDue, bool());
	MOCK_METHOD0(startCountingNextStep, void());
};

#endif /* TIMER_16_BIT_MOCK_H */
