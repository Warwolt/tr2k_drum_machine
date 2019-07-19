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
	static constexpr u32 clockFrequency = 16000000;
	static constexpr u8  secondsPerMinute = 60;
	static constexpr u16 pulsesPerQuarterNote = 480;
	static constexpr u16 pulsesPerSixteenthNote = pulsesPerQuarterNote / 4;
	static constexpr u32 scalingConstant = clockFrequency * secondsPerMinute / pulsesPerQuarterNote;

	Timer16Bit& timer16Bit;
	u16 countedPulses = 0;
};

#endif /* TEMPO_TIMER_16_BIT_H */
