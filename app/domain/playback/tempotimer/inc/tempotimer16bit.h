/*
****************************************************************************************************
* brief : Timer16Bit implementation of TempoTimer interface
****************************************************************************************************
*/

#ifndef TEMPO_TIMER_16_BIT_H
#define TEMPO_TIMER_16_BIT_H

#include "linuxtypes.h"
#include "tempotimer.h"
#include "timer16bit.h"

class TempoTimer16Bit : TempoTimer
{
public:
	TempoTimer16Bit(Timer16Bit& timer16Bit);
	void setTempo(BeatsPerMinute bpm);
	void start();
	void stop();
	void clear();
	void countPulse();
	bool playbackStepIsDue();
	void startCountingNextStep();

private:
	static constexpr u32 clockFrequency = 16e6;
	static constexpr u8  secondsPerMinute = 60;
	static constexpr u16 pulsesPerQuarterNote = 480;
	static constexpr u16 pulsesPerSixteenthNote = pulsesPerQuarterNote / 4;
	static constexpr u32 scalingConstant = clockFrequency * secondsPerMinute / pulsesPerQuarterNote;

	Timer16Bit& timer16Bit;
	u16 countedPulses = 0;
};

#endif /* TEMPO_TIMER_16_BIT_H */
