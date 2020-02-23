/*
****************************************************************************************************
* brief : Unit tests for PatternEditView
****************************************************************************************************
*/

#include "gtest/gtest.h"
#include "traceprint.h"
#include "linuxtypes.h"
#include "PatternEditView.h"
#include "TempoTimerMock.h"
#include "LedGroupMock.h"
#include "ButtonGroupMock.h"

using ::testing::Return;
using ::testing::NiceMock;

class PatternEditViewTest : public ::testing::Test
{
public:
    static constexpr u8 channelSelectButton = 0;
    static constexpr u8 numStepLeds = 16;

    /* Dependencies */
    // mocks
    NiceMock<LedGroupMock> stepLedMock;
    NiceMock<ButtonGroupMock> editControlButtons;
    NiceMock<TempoTimerMock> timerMock;
    // concretes
    RhythmPlaybackManager playbackManager {timerMock};
    RhythmPatternManager patternManager;
    PatternEditController editController {playbackManager, patternManager};

    /* Class under test */
    PatternEditView editView {editController, editControlButtons, stepLedMock};

    void expectAllLedsCleared()
    {
        const u8 numSteps = patternManager.getActivePattern().length;
        for (int i = 0; i < numSteps; i++)
        {
            EXPECT_CALL(stepLedMock, clearLed(i));
        }
    }

    void expectLedState(u8 state)
    {
        for (int i = 0; i < numStepLeds; i++)
        {
            if (state & (0x1 << i))
                EXPECT_CALL(stepLedMock, setLed(i));
            else
                EXPECT_CALL(stepLedMock, clearLed(i));
        }
    }

    void holdDownChannelSelectButton()
    {
        EXPECT_CALL(editControlButtons, buttonIsDown(channelSelectButton)).WillOnce(Return(true));
    }
};

/* Pattern edit mode tests -----------------------------------------------------------------------*/
TEST_F(PatternEditViewTest, Clears_all_leds_if_pattern_empty_during_edit_mode)
{
    patternManager.clearActivePattern();
    expectAllLedsCleared();
    editView.update();
}

TEST_F(PatternEditViewTest, Sets_leds_corresponding_to_active_steps_during_edit_mode)
{
    patternManager.selectActivePattern(3); // just making sure active pattern isn't just pattern 0
    patternManager.toggleActivePatternStep(0);
    patternManager.toggleActivePatternStep(2);
    EXPECT_CALL(stepLedMock, setLed(0));
    EXPECT_CALL(stepLedMock, setLed(2));
    editView.update();
}

TEST_F(PatternEditViewTest, Inverts_led_for_playback_position_if_playback_ongoing_during_edit_mode)
{
    /* Start playback, and move playback position forward once */
    playbackManager.startPlayback(); // playback is now ongoing
    EXPECT_CALL(timerMock, playbackStepIsDue()).WillOnce(Return(true));
    playbackManager.handlePlayback(); // playback step is now 1

    /* Setup pattern state to be drawn on LEDs */
    patternManager.toggleActivePatternStep(0);
    patternManager.toggleActivePatternStep(1); // same as playback position, so will be inverted

    /* Update with view with playback ongoing */
    expectLedState(0x1); // not 0x3 since step 1 is inverted
    editView.update();
}

/* Pattern select mode tests ---------------------------------------------------------------------*/
TEST_F(PatternEditViewTest, Lights_up_leds_for_non_empty_patterns_during_channel_select_mode)
{
    /* Setup patterns */
    // pattern 0
    patternManager.selectActivePattern(0);
    patternManager.toggleActivePatternStep(0);
    // pattern 1
    patternManager.selectActivePattern(1);
    patternManager.toggleActivePatternStep(0);

    /* Check output */
    holdDownChannelSelectButton();
    expectLedState(0x3); // first two leds should be set
    editView.update();
}
