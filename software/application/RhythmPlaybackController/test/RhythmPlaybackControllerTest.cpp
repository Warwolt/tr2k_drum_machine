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

class RhythmPlaybackControllerTest : public ::testing::Test
{
public:
	TempoTimerMock timerMock;
	RhythmPlaybackController controller = RhythmPlaybackController(timerMock);
};

TEST_F(RhythmPlaybackControllerTest, Set_tempo_delegated_to_tempo_timer)
{
	BeatsPerMinute bpm = BeatsPerMinute(123);
	EXPECT_CALL(timerMock, setTempo(bpm));
	controller.setTempo(bpm);
}

TEST_F(RhythmPlaybackControllerTest, Restart_playback_clears_then_starts_tempo_timer)
{
	EXPECT_CALL(timerMock, clear());
	EXPECT_CALL(timerMock, start());
	controller.restartPlayback();
}

TEST_F(RhythmPlaybackControllerTest, Stop_playback_stops_tempo_timer)
{
	EXPECT_CALL(timerMock, stop());
	controller.stopPlayback();
}

TEST_F(RhythmPlaybackControllerTest, Continue_playback_starts_tempo_timer_without_clearing)
{
	EXPECT_CALL(timerMock, start());
	controller.continuePlayback();
}
