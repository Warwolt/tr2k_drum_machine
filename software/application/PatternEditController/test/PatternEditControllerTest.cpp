/*
****************************************************************************************************
* @brief  Test for PatternEditController
****************************************************************************************************
*/

#include "gtest/gtest.h"
#include "traceprint.h"
#include "linuxtypes.h"
#include "PatternEditController.h"
#include "TempoTimerMock.h"
#include "RhythmPlaybackManager.h"

using ::testing::Return;
using ::testing::NiceMock;

class PatternEditControllerTest : public ::testing::Test
{
public:
    /* Dependencies */
    RhythmPatternManager patternManager;
    NiceMock<TempoTimerMock> timerMock;
    RhythmPlaybackManager playbackManager {timerMock};

    /* Class under test */
    PatternEditController controller {playbackManager, patternManager};
};

TEST_F(PatternEditControllerTest, Active_rhythm_pattern_can_be_accessed)
{
    patternManager.selectActivePattern(1);
    patternManager.toggleActivePatternStep(3);
    EXPECT_EQ(controller.getActivePattern().state, 0x8);
}

TEST_F(PatternEditControllerTest, Stopped_playback_can_be_queried)
{
    playbackManager.stopPlayback();
    EXPECT_FALSE(controller.playbackIsOngoing());
}

TEST_F(PatternEditControllerTest, Ongoing_playback_can_be_queried)
{
    playbackManager.restartPlayback();
    EXPECT_TRUE(controller.playbackIsOngoing());
}


TEST_F(PatternEditControllerTest, Playback_position_can_be_accessed)
{
    /* Move playback step forward once */
    playbackManager.restartPlayback();
    EXPECT_CALL(timerMock, playbackStepIsDue()).WillOnce(Return(true));
    playbackManager.handlePlayback(); // this should now have move forward playback position

    /* Check that playback position is correct */
    EXPECT_EQ(controller.getPlaybackPosition(), 1);
}

TEST_F(PatternEditControllerTest, Patterns_can_be_accessed)
{
    patternManager.selectActivePattern(2);
    patternManager.toggleActivePatternStep(4);
    EXPECT_EQ(controller.getPattern(2).state, 0x10);
}

TEST_F(PatternEditControllerTest, Active_pattern_steps_can_be_toggled)
{
    patternManager.selectActivePattern(1);
    controller.toggleActivePatternStep(2);
    EXPECT_EQ(controller.getPattern(1).state, 0x4);
}
