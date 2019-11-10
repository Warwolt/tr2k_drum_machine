/*
****************************************************************************************************
* brief : Unit tests for RhythmPlaybackController
****************************************************************************************************
*/

#include "gtest/gtest.h"
#include "traceprint.h"
#include "linuxtypes.h"
#include "RhythmPlaybackController.h"
#include "TempoTimerMock.h"

class TestRhythmPlaybackController : public ::testing::Test
{
public:
	TempoTimerMock timerMock;
	RhythmPlaybackController controller = RhythmPlaybackController(timerMock);
};

TEST_F(TestRhythmPlaybackController, Set_tempo_delegated_to_tempo_timer)
{
	BeatsPerMinute bpm = BeatsPerMinute(123);
	EXPECT_CALL(timerMock, setTempo(bpm));
	controller.setTempo(bpm);
}

TEST_F(TestRhythmPlaybackController, Restart_playback_clears_then_starts_tempo_timer)
{
	EXPECT_CALL(timerMock, clear());
	EXPECT_CALL(timerMock, start());
	controller.restartPlayback();
}

TEST_F(TestRhythmPlaybackController, Stop_playback_stops_tempo_timer)
{
	EXPECT_CALL(timerMock, stop());
	controller.stopPlayback();
}

TEST_F(TestRhythmPlaybackController, Continue_playback_starts_tempo_timer_without_clearing)
{
	EXPECT_CALL(timerMock, start());
	controller.continuePlayback();
}
