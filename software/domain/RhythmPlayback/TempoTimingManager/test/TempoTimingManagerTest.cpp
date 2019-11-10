/*
****************************************************************************************************
* brief : Unit tests for TempoTimingManager
****************************************************************************************************
*/

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "traceprint.h"
#include "TempoTimingManager.h"
#include "TempoTimerMock.h"

using testing::NiceMock;
using testing::Return;

static bool callbackWasCalled[2];

class TestTempoTimingManager : public ::testing::Test
{
public:
	NiceMock<TempoTimerMock> mockTimer;
	TempoTimingManager timingManager = TempoTimingManager(mockTimer);

	void SetUp()
	{
		for (int i = 0; i < 2; i++)
		{
			callbackWasCalled[i] = false;
		}
	}
};

TEST_F(TestTempoTimingManager, A_maximum_amount_of_callbacks_can_be_registered)
{
	for (size_t i = 0; i < timingManager.maxNumHandlers; i++)
	{
		timingManager.addPlaybackStepHandler([]{ callbackWasCalled[0] = true; });
	}
	timingManager.addPlaybackStepHandler([]{ callbackWasCalled[1] = true; });

	EXPECT_CALL(mockTimer, playbackStepIsDue()).WillOnce(Return(true));
	timingManager.handlePlayback();

	EXPECT_TRUE(callbackWasCalled[0]);
	EXPECT_FALSE(callbackWasCalled[1]);
}

TEST_F(TestTempoTimingManager, If_playback_step_not_due_then_handling_playback_does_nothing)
{
	EXPECT_CALL(mockTimer, playbackStepIsDue()).WillOnce(Return(false));
	timingManager.handlePlayback();
}

TEST_F(TestTempoTimingManager, If_playback_step_is_due_then_all_callbacks_are_called)
{
	timingManager.addPlaybackStepHandler([]{ callbackWasCalled[0] = true; });
	timingManager.addPlaybackStepHandler([]{ callbackWasCalled[1] = true; });

	EXPECT_CALL(mockTimer, playbackStepIsDue()).WillOnce(Return(true));
	timingManager.handlePlayback();

	EXPECT_TRUE(callbackWasCalled[0]);
	EXPECT_TRUE(callbackWasCalled[1]);
}

TEST_F(TestTempoTimingManager, After_handled_current_playback_step_starts_counting_next)
{
	EXPECT_CALL(mockTimer, playbackStepIsDue()).WillOnce(Return(true));
	EXPECT_CALL(mockTimer, startCountingNextStep());
	timingManager.handlePlayback();
}
