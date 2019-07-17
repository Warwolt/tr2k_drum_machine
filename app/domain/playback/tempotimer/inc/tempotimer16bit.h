/*
****************************************************************************************************
* brief : Timer16Bit implementation of TempoTimer interface
****************************************************************************************************
*/

#ifndef TEMPO_TIMER_16_BIT_H
#define TEMPO_TIMER_16_BIT_H

#include "linuxtypes.h"
#include "timer16bit.h"

class TempoTimer16Bit
{
public:
	TempoTimer16Bit(Timer16Bit& timer16bit);
	void countPulse();
	bool playbackStepIsDue();
	void resetPlaybackStepDue();

private:
	static constexpr u16 pulsesPerSixteenthNote = 120;
	Timer16Bit& timer16bit;
	u16 countedPulses = 0;
};

#endif /* TEMPO_TIMER_16_BIT_H */
