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
using ::testing::_;

class PatternEditViewTest : public ::testing::Test
{
public:
    static constexpr u8 channelSelectButton = 0;
    static constexpr u8 numStepLeds = 16;

    /* Dependencies */
    // mocks
    NiceMock<LedGroupMock> stepLedMock;
    NiceMock<ButtonGroupMock> editControlButtons;
    NiceMock<ButtonGroupMock> stepButtons;
    NiceMock<TempoTimerMock> timerMock;
    // concretes
    RhythmPlaybackManager playbackManager {timerMock};
    RhythmPatternManager patternManager;
    PatternEditController editController {playbackManager, patternManager};

    /* Class under test */
    PatternEditView editView {editController, editControlButtons, stepButtons, stepLedMock};

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

    void setButtonStates(u8 state)
    {
        for (int i = 0; i < numStepLeds; i++)
        {
            bool isPressed = (state & (0x1 << i));
            EXPECT_CALL(stepButtons, buttonPressedNow(i)).WillOnce(Return(isPressed));
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
    patternManager.toggleActivePatternStep(9);
    EXPECT_CALL(stepLedMock, setLed(0));
    EXPECT_CALL(stepLedMock, setLed(9));
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

TEST_F(PatternEditViewTest, Step_buttons_toggle_state_in_active_pattern_during_edit_mode)
{
    patternManager.selectActivePattern(2); // just making sure active pattern isn't just pattern 0

    /* Press buttons to toggle pattern state*/
    setButtonStates(0x5);

    /* Check that pattern state was changed */
    editView.update();
    EXPECT_EQ(patternManager.getActivePattern().state, 0x5);
}

/* Pattern select mode tests ---------------------------------------------------------------------*/
TEST_F(PatternEditViewTest, Lights_up_led_for_active_pattern)
{
    patternManager.selectActivePattern(1);
    holdDownChannelSelectButton();
    expectLedState(0x2);
    editView.update();
}

TEST_F(PatternEditViewTest, Channel_can_be_selected_with_step_buttons_during_channel_select_mode)
{
    /* Setup patterns */
    // pattern 0
    patternManager.selectActivePattern(0);
    patternManager.toggleActivePatternStep(1); // 0x2
    // pattern 1
    patternManager.selectActivePattern(1);
    patternManager.toggleActivePatternStep(2); // 0x4

    /* Check that pattern 1 is selected */
    holdDownChannelSelectButton();
    setButtonStates(0x1 << 1); // press button 1 to select channel 1
    editView.update();
    EXPECT_EQ(patternManager.getActivePattern().state, 0x4); // pattern 1 state
}
