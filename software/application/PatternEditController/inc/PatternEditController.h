/*
****************************************************************************************************
* @brief  MVC Controller for rhythm pattern editing
****************************************************************************************************
*/

#ifndef PATTERN_EDIT_CONTROLLER
#define PATTERN_EDIT_CONTROLLER

#include "linuxtypes.h"
#include "RhythmPatternManager.h"
#include "RhythmPlaybackManager.h"

class PatternEditController
{
public:
    PatternEditController(RhythmPlaybackManager& playbackManager,
        RhythmPatternManager& patternManager);
    bool playbackIsOngoing();
    RhythmPattern getActivePattern();
    RhythmPattern getPattern(u8 patternNum);
    u8 getPlaybackPosition();

private:
    RhythmPlaybackManager& playbackManager;
    RhythmPatternManager& patternManager;
};

#endif /* PATTERN_EDIT_CONTROLLER */
