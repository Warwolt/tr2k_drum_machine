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
#include <vector>
#include <algorithm>

using ::testing::Return;
using ::testing::NiceMock;
using ::testing::_;

class PatternEditViewTest : public ::testing::Test
{
public:
    static constexpr u8 channelSelectButton = 0;
    static constexpr u8 patternClearButton = 1;
    static constexpr u8 maxNumPatternSteps = 16;

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


    template <typename T>
    bool numberIsInVector(int num, std::vector<T>& vec)
    {
        return std::any_of(vec.begin(), vec.end(), [=](int x){ return x == num; });
    }

    void expectAllLedsCleared()
    {
        const u8 numSteps = patternManager.getActivePattern().length;
        for (int i = 0; i < numSteps; i++)
        {
            EXPECT_CALL(stepLedMock, clearLed(i));
        }
    }

    /**
     * @brief Expects these leds to be set and all others to be cleared
     */
    template <typename... Args>
    void expectLedsToBeSet(Args... ledNums)
    {
        auto ledsToSet = std::vector<int>({ledNums...});
        for (int num = 0; num < maxNumPatternSteps; num++)
        {
            if (numberIsInVector(num, ledsToSet))
            {
                EXPECT_CALL(stepLedMock, setLed(num));
            }
            else
            {
                EXPECT_CALL(stepLedMock, clearLed(num));
            }
        }
    }

    /**
     * @brief Toggle a number of steps in active pattern at same time
     */
    template <typename... Args>
    void toggleActivePatternSteps(Args... stepNums)
    {
        for (auto& stepNum : std::vector<int>({stepNums...}))
        {
            patternManager.toggleActivePatternStep(stepNum);
        }

    }

    /**
     * @brief Set which buttons to press using variadic args
     */
    template <typename... Args>
    void setStepButtons(Args... stepButtonNums)
    {
        auto buttonsToPress = std::vector<int>({stepButtonNums...});
        for (int num = 0; num < maxNumPatternSteps; num++)
        {
            bool isPressed = (numberIsInVector(num, buttonsToPress));
            EXPECT_CALL(stepButtons, buttonPressedNow(num)).WillOnce(Return(isPressed));
        }
    }

    void holdDownChannelSelectButton(bool isDown)
    {
        EXPECT_CALL(editControlButtons, buttonIsDown(channelSelectButton)).WillOnce(Return(isDown));
    }

    void holdDownClearPatternButton(bool isDown)
    {
        EXPECT_CALL(editControlButtons, buttonIsDown(patternClearButton)).WillOnce(Return(isDown));
    }

    void startPlaybackAndMovePlaybackPositionTo(u8 position)
    {
        playbackManager.startPlayback(); // playback is now ongoing
        for (int i = 0; i < position; i++)
        {
            EXPECT_CALL(timerMock, playbackStepIsDue()).WillOnce(Return(true));
            playbackManager.handlePlayback(); // moves playback forward once
        }
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
    /* Toggle some pattern steps */
    patternManager.selectActivePattern(3); // just making sure active pattern isn't just pattern 0
    toggleActivePatternSteps(0, 9);

    /* Expect corresponding leds to be lit */
    expectLedsToBeSet(0, 9);
    editView.update();
}

TEST_F(PatternEditViewTest, Inverts_led_for_playback_position_if_playback_ongoing_during_edit_mode)
{
    /* Set playback position to 1 and toggle pattern setps */
    startPlaybackAndMovePlaybackPositionTo(1);
    toggleActivePatternSteps(0, 1, 2);

    /* Expect playback position to invert step 1 led state */
    expectLedsToBeSet(0, 2);
    editView.update();
}

TEST_F(PatternEditViewTest, Step_buttons_toggle_state_in_active_pattern_during_edit_mode)
{
    /* Push some step buttons */
    patternManager.selectActivePattern(2);
    setStepButtons(0, 2);

    /* Pattern state should've set accordingly */
    editView.update();
    EXPECT_EQ(patternManager.getActivePattern().state, 0x5);
}

TEST_F(PatternEditViewTest, Clear_button_clears_current_step_if_playback_ongoing_during_edit_mode)
{
    /* Write to pattern */
    patternManager.selectActivePattern(5);
    toggleActivePatternSteps(0, 1, 2, 3); // 0xF

    /* Move playback position forward and press clear button */
    startPlaybackAndMovePlaybackPositionTo(3);
    holdDownChannelSelectButton(false); // don't press channel select
    holdDownClearPatternButton(true); // should clear bit 3 in active pattern state

    /* After update, step should've been cleared */
    editView.update();
    EXPECT_EQ(patternManager.getActivePattern().state, 0x7);
}

TEST_F(PatternEditViewTest, Clear_button_clears_whole_pattern_if_playback_stopped_during_edit_mode)
{
    /* Write to pattern */
    patternManager.selectActivePattern(5);
    toggleActivePatternSteps(0, 1, 2, 3); // 0xF

    /* Move playback position forward and press clear button */
    playbackManager.stopPlayback();
    holdDownChannelSelectButton(false); // don't press channel select
    holdDownClearPatternButton(true); // should clear bit 3 in active pattern state

    /* After update, step should've been cleared */
    editView.update();
    EXPECT_EQ(patternManager.getActivePattern().state, 0x0);
}


/* Pattern select mode tests ---------------------------------------------------------------------*/
TEST_F(PatternEditViewTest, Lights_up_led_for_active_pattern)
{
    patternManager.selectActivePattern(1);
    holdDownChannelSelectButton(true);
    expectLedsToBeSet(1);
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
    holdDownChannelSelectButton(true);
    setStepButtons(1);
    editView.update();
    EXPECT_EQ(patternManager.getActivePattern().state, 0x4); // pattern 1 state
}
