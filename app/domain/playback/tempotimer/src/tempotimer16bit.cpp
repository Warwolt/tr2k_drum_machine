/*
****************************************************************************************************
* brief : Timer16Bit implementation of TempoTimer interface
****************************************************************************************************
*/

#include "tempotimer16Bit.h"

TempoTimer16Bit::TempoTimer16Bit(Timer16Bit& timer16Bit) : timer16Bit(timer16Bit)
{
	timer16Bit.setPrescaler(Timer16Bit::PrescaleOption::_1);
}

/**
 * Sets a tempo in beats per minutes by calculating the corresponding
 * 16-bit timer period and assigning it.
 */
void TempoTimer16Bit::setTempo(BeatsPerMinute bpm)
{
	timer16Bit.setPeriod(33333);
}

/**
 * Starts the tempo timer by starting the underlying 16-bit timer.
 */
void TempoTimer16Bit::start()
{
	timer16Bit.start();
}

/**
 * Stops the tempo timer by stopping the underlying 16-bit timer.
 */
void TempoTimer16Bit::stop()
{
	timer16Bit.stop();
}

/**
 * Clears the tempo timer by clearing the underlying 16-bit timer.
 */
void TempoTimer16Bit::clear()
{
	timer16Bit.clear();
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
