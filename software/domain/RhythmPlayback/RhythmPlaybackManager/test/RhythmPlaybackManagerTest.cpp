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

// Spy function variables
static bool handlePlaybackWasCalled[2];
static bool resetPlaybackWasCalled[2];

class RhythmPlaybackManagerTest : public ::testing::Test
{
public:
	NiceMock<TempoTimerMock> mockTimer;
	RhythmPlaybackManager playbackManager = RhythmPlaybackManager(mockTimer);

	void SetUp()
	{
		for (int i = 0; i < 2; i++)
		{
			handlePlaybackWasCalled[i] = false;
		}
	}

	void addPlaybackHandler0()
	{
		playbackManager.addPlaybackHandler({
			.handlePlaybackStep = []{ handlePlaybackWasCalled[0] = true; },
			.resetPlayback = []{ resetPlaybackWasCalled[0] = true; },
		});
	}

	void addPlaybackHandler1()
	{
		playbackManager.addPlaybackHandler({
			.handlePlaybackStep = []{ handlePlaybackWasCalled[1] = true; },
			.resetPlayback = []{ resetPlaybackWasCalled[1] = true; },
		});
	}

	void signalNextPlaybackStep()
	{
		EXPECT_CALL(mockTimer, playbackStepIsDue()).WillOnce(Return(true));
	}
};

TEST_F(RhythmPlaybackManagerTest, A_maximum_amount_of_callbacks_can_be_registered)
{
	for (size_t i = 0; i < playbackManager.maxNumHandlers; i++)
	{
		playbackManager.addPlaybackHandler([]{ handlePlaybackWasCalled[0] = true; });
	}
	playbackManager.addPlaybackHandler([]{ handlePlaybackWasCalled[1] = true; });

	signalNextPlaybackStep();
	playbackManager.handlePlayback();

	EXPECT_TRUE(handlePlaybackWasCalled[0]);
	EXPECT_FALSE(handlePlaybackWasCalled[1]);
}

TEST_F(RhythmPlaybackManagerTest, Starting_playback_clears_and_restarts_tempo_timer)
{
	EXPECT_CALL(mockTimer, clear());
	EXPECT_CALL(mockTimer, start());
	playbackManager.restartPlayback();
}

TEST_F(RhythmPlaybackManagerTest, Stopping_playback_stops_tempo_timer)
{
	EXPECT_CALL(mockTimer, stop());
	playbackManager.stopPlayback();
}

TEST_F(RhythmPlaybackManagerTest, Playback_ongoing_if_its_been_started)
{
	playbackManager.restartPlayback();
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
	addPlaybackHandler0();
	addPlaybackHandler1();

	signalNextPlaybackStep();
	playbackManager.handlePlayback();

	EXPECT_TRUE(handlePlaybackWasCalled[0]);
	EXPECT_TRUE(handlePlaybackWasCalled[1]);
}

TEST_F(RhythmPlaybackManagerTest, All_playback_handlers_are_reset_when_playback_is_reset)
{
	addPlaybackHandler0();
	addPlaybackHandler1();

	playbackManager.restartPlayback();

	EXPECT_TRUE(resetPlaybackWasCalled[0]);
	EXPECT_TRUE(resetPlaybackWasCalled[1]);
}

TEST_F(RhythmPlaybackManagerTest, After_handled_current_playback_step_starts_counting_next)
{
	signalNextPlaybackStep();
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

TEST_F(RhythmPlaybackManagerTest, Playback_position_reset_when_playback_reset)
{
	/* Move playback position forward */
	signalNextPlaybackStep();
	playbackManager.handlePlayback();

	/* After restarting playback, position should be reset */
	playbackManager.restartPlayback();
	EXPECT_EQ(playbackManager.getPlaybackPosition(), 0);
}
