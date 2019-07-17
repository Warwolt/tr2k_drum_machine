/*
****************************************************************************************************
* brief : Timer16Bit implementation of TempoTimer interface
****************************************************************************************************
*/

#include "tempotimer16bit.h"

TempoTimer16Bit::TempoTimer16Bit(Timer16Bit& timer16bit) : timer16bit(timer16bit)
{
	timer16bit.setPrescaler(Timer16Bit::PrescaleOption::_1);
}

void TempoTimer16Bit::countPulse()
{
	countedPulses++;
}

bool TempoTimer16Bit::playbackStepIsDue()
{
	return (countedPulses >= pulsesPerSixteenthNote) ? true : false;
}

void TempoTimer16Bit::resetPlaybackStepDue()
{
	countedPulses = 0;
}

