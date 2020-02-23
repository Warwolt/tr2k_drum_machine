/*
****************************************************************************************************
* brief : Unit tests for PatternEditView
****************************************************************************************************
*/

#include "gtest/gtest.h"
#include "traceprint.h"
#include "linuxtypes.h"
#include "ButtonGroupMock.h"
#include "TempoTimerMock.h"
#include "RhythmPlaybackManager.h"
#include "PlaybackControlView.h"

using ::testing::Return;
using ::testing::NiceMock;
using ::testing::_;

class PlaybackControlViewTest : public ::testing::Test
{
public:
    static constexpr u8 startButton = 0;
    static constexpr u8 stopButton = 1;

    NiceMock<TempoTimerMock> mockTimer;
    NiceMock<ButtonGroupMock> mockTransportButtons;
    RhythmPlaybackManager playbackManager {mockTimer};
    RhythmPlaybackController playbackController {playbackManager, mockTimer};
    PlaybackControlView playbackView {playbackController, mockTransportButtons};

    void pressStartButton()
    {
        EXPECT_CALL(mockTransportButtons, buttonPressedNow(startButton)).WillOnce(Return(true));
        EXPECT_CALL(mockTransportButtons, buttonPressedNow(stopButton)).WillOnce(Return(false));
    }

    void pressStopButton()
    {
        EXPECT_CALL(mockTransportButtons, buttonPressedNow(stopButton)).WillOnce(Return(true));
        EXPECT_CALL(mockTransportButtons, buttonPressedNow(startButton)).WillOnce(Return(false));
    }
};

TEST_F(PlaybackControlViewTest, Pressing_play_button_starts_playback)
{
    playbackController.stopPlayback(); // make sure we're stopped
    pressStartButton();
    playbackView.update();
    EXPECT_TRUE(playbackManager.playbackIsOngoing());
}

TEST_F(PlaybackControlViewTest, Pressing_stop_button_when_playback_ongoing_pauses_playback)
{
    playbackController.restartPlayback(); // make sure wer're playing
    pressStopButton();
    playbackView.update();
    EXPECT_FALSE(playbackManager.playbackIsOngoing());
}

TEST_F(PlaybackControlViewTest, Pressing_stop_button_when_playback_paused_resumes_playback)
{
    playbackController.stopPlayback(); // make sure we're stopped
    pressStopButton();
    playbackView.update();
    EXPECT_TRUE(playbackManager.playbackIsOngoing());
}
