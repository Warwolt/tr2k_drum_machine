/*
****************************************************************************************************
* brief : Interface view for controlling rhythm playback
****************************************************************************************************
*/

#ifndef PLAYBACK_CONTROL_VIEW_H
#define PLAYBACK_CONTROL_VIEW_H

#include "ButtonGroup.h"
#include "RhythmPlaybackController.h"

class PlaybackControlView
{
public:
    PlaybackControlView(RhythmPlaybackController& playbackController, ButtonGroup& transportButtons);
    void update();

private:
    static constexpr u8 startButton = 0;
    static constexpr u8 stopButton = 1;

    RhythmPlaybackController& playbackController;
    ButtonGroup& transportButtons;
};

#endif /* PLAYBACK_CONTROL_VIEW_H */
