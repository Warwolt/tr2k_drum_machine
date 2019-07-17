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

/**
 * Starts the tempo timer by starting the underlying 16-bit timer.
 */
void TempoTimer16Bit::start()
{
	timer16bit.start();
}

/**
 * Stops the tempo timer by starting the underlying 16-bit timer.
 */
void TempoTimer16Bit::stop()
{
	timer16bit.stop();
}

/**
 * Clears the tempo timer by starting the underlying 16-bit timer.
 */
void TempoTimer16Bit::clear()
{
	timer16bit.clear();
}

/**
 * Count a tempo pulse (pulses-per-sixteenth-note pulses).
 * Used to track when the period for a sixteenth note has elapsed,
 * and the next playback step is due for execution.
 */
void TempoTimer16Bit::countPulse()
{
	countedPulses++;
}

/**
 * Should be polled continously by the client.
 * @return true if the next playback step is due for execution.
 */
bool TempoTimer16Bit::playbackStepIsDue()
{
	return (countedPulses >= pulsesPerSixteenthNote) ? true : false;
}

/**
 * Used to signal that current playback step has been handled and that the next
 * playback step wait period should start elapsing. Counted pulses are reset.
 */
void TempoTimer16Bit::resetPlaybackStepDue()
{
	countedPulses = 0;
}

