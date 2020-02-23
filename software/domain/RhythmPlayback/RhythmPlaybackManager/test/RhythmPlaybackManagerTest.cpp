/*
****************************************************************************************************
* brief : Unit tests for RhythmPlaybackManager
****************************************************************************************************
*/

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "traceprint.h"
#include "RhythmPlaybackManager.h"
#include "TempoTimerMock.h"

using testing::NiceMock;
using testing::Return;

static bool callbackWasCalled[2];

class RhythmPlaybackManagerTest : public ::testing::Test
{
public:
	NiceMock<TempoTimerMock> mockTimer;
	RhythmPlaybackManager playbackManager = RhythmPlaybackManager(mockTimer);

	void SetUp()
	{
		for (int i = 0; i < 2; i++)
		{
			callbackWasCalled[i] = false;
		}
	}
};

TEST_F(RhythmPlaybackManagerTest, A_maximum_amount_of_callbacks_can_be_registered)
{
	for (size_t i = 0; i < playbackManager.maxNumHandlers; i++)
	{
		playbackManager.addPlaybackStepHandler([]{ callbackWasCalled[0] = true; });
	}
	playbackManager.addPlaybackStepHandler([]{ callbackWasCalled[1] = true; });

	EXPECT_CALL(mockTimer, playbackStepIsDue()).WillOnce(Return(true));
	playbackManager.handlePlayback();

	EXPECT_TRUE(callbackWasCalled[0]);
	EXPECT_FALSE(callbackWasCalled[1]);
}

TEST_F(RhythmPlaybackManagerTest, Starting_playback_clears_and_restarts_tempo_timer)
{
	EXPECT_CALL(mockTimer, clear());
	EXPECT_CALL(mockTimer, start());
	playbackManager.startPlayback();
}

TEST_F(RhythmPlaybackManagerTest, Stopping_playback_stops_tempo_timer)
{
	EXPECT_CALL(mockTimer, stop());
	playbackManager.stopPlayback();
}

TEST_F(RhythmPlaybackManagerTest, Playback_ongoing_if_its_been_started)
{
	playbackManager.startPlayback();
	EXPECT_TRUE(playbackManager.playbackIsOngoing());
}

TEST_F(RhythmPlaybackManagerTest, Playback_not_ongoing_if_its_been_stopped)
{
	playbackManager.stopPlayback();
	EXPECT_FALSE(playbackManager.playbackIsOngoing());
}

TEST_F(RhythmPlaybackManagerTest, If_playback_step_not_due_then_handling_playback_does_nothing)
{
	EXPECT_CALL(mockTimer, playbackStepIsDue()).WillOnce(Return(false));
	playbackManager.handlePlayback();
}

TEST_F(RhythmPlaybackManagerTest, If_playback_step_is_due_then_all_callbacks_are_called)
{
	playbackManager.addPlaybackStepHandler([]{ callbackWasCalled[0] = true; });
	playbackManager.addPlaybackStepHandler([]{ callbackWasCalled[1] = true; });

	EXPECT_CALL(mockTimer, playbackStepIsDue()).WillOnce(Return(true));
	playbackManager.handlePlayback();

	EXPECT_TRUE(callbackWasCalled[0]);
	EXPECT_TRUE(callbackWasCalled[1]);
}

TEST_F(RhythmPlaybackManagerTest, After_handled_current_playback_step_starts_counting_next)
{
	EXPECT_CALL(mockTimer, playbackStepIsDue()).WillOnce(Return(true));
	EXPECT_CALL(mockTimer, startCountingNextStep());
	playbackManager.handlePlayback();
}

TEST_F(RhythmPlaybackManagerTest, Playback_position_increments_during_handled_playback_step)
{
	EXPECT_CALL(mockTimer, playbackStepIsDue()).WillRepeatedly(Return(true));
	// test playback position 0 - 15
	for (int i = 0; i < 16; i++)
	{
		EXPECT_EQ(playbackManager.getPlaybackPosition(), i);
		playbackManager.handlePlayback();
	}
	// test that position wraps around to 0
	EXPECT_EQ(playbackManager.getPlaybackPosition(), 0);
	playbackManager.handlePlayback();
}
