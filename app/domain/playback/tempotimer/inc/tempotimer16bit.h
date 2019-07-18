/*
****************************************************************************************************
* brief : Timer16Bit implementation of TempoTimer interface
****************************************************************************************************
*/

#ifndef TEMPO_TIMER_16_BIT_H
#define TEMPO_TIMER_16_BIT_H

#include "linuxtypes.h"
#include "timer16bit.h"
#include "beatsperminute.h"

class TempoTimer16Bit
{
public:
	TempoTimer16Bit(Timer16Bit& timer16Bit);
	void setTempo(BeatsPerMinute bpm);
	void start();
	void stop();
	void clear();
	void countPulse();
	bool playbackStepIsDue();
	void resetPlaybackStepDue();

private:
	static constexpr u16 pulsesPerSixteenthNote = 120;
	Timer16Bit& timer16Bit;
	u16 countedPulses = 0;
};

#endif /* TEMPO_TIMER_16_BIT_H */
