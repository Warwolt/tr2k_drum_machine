/*
****************************************************************************************************
* brief : Unit tests for Timer16Bit implementation of TempoTimer interface
****************************************************************************************************
*/

#include "gtest/gtest.h"
#include "traceprint.h"
#include "linuxtypes.h"
#include "tempotimer16bit.h"
#include "timer16bitmock.h"

using testing::NiceMock;

static constexpr u16 pulsesPerSixteenthNote = 120;

class TestTempoTimer16Bit : public ::testing::Test
{
public:
	NiceMock<Timer16BitMock> timer16Mock;
	TempoTimer16Bit tempoTimer = TempoTimer16Bit(timer16Mock);

	void countTempoPulses(u16 numPulses)
	{
		for(size_t i = 0; i < numPulses; i++)
		{
			tempoTimer.countPulse();
		}
	}
};

TEST_F(TestTempoTimer16Bit, Prescaler_set_to_1_in_constructor)
{
	EXPECT_CALL(timer16Mock, setPrescaler(Timer16Bit::PrescaleOption::_1));
	TempoTimer16Bit tempoTimer(timer16Mock); // mock called from constructor
}

TEST_F(TestTempoTimer16Bit, Playback_not_due_if_not_enough_tempo_pulses_counted)
{
	EXPECT_FALSE(tempoTimer.playbackStepIsDue());
}

TEST_F(TestTempoTimer16Bit, Playback_is_due_if_enough_pulses_counted)
{
	countTempoPulses(pulsesPerSixteenthNote);
	EXPECT_TRUE(tempoTimer.playbackStepIsDue());
}

TEST_F(TestTempoTimer16Bit, Playback_no_longer_due_after_reset)
{
	countTempoPulses(pulsesPerSixteenthNote);
	tempoTimer.resetPlaybackStepDue();
	EXPECT_FALSE(tempoTimer.playbackStepIsDue());
}