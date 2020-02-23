/*
****************************************************************************************************
* brief : Interface view for controlling rhythm playback
****************************************************************************************************
*/

#include "PlaybackControlView.h"

PlaybackControlView::PlaybackControlView(RhythmPlaybackController& controller, ButtonGroup& buttons)
    : playbackController(controller), transportButtons(buttons) {}

void PlaybackControlView::update()
{
    if (transportButtons.buttonPressedNow(startButton))
    {
        playbackController.restartPlayback();
    }

    if (transportButtons.buttonPressedNow(stopButton))
    {
        if (playbackController.playbackIsOngoing())
        {
            playbackController.stopPlayback();
        }
        else
        {
            playbackController.continuePlayback();
        }

    }
}
