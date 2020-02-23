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
    u8 getActivePatternNum() const;
    void selectActivePattern(u8 patternNum);
    RhythmPattern getPattern(u8 patternNum);
    void toggleActivePatternStep(u8 stepNum);
    u8 getPlaybackPosition();

private:
    RhythmPlaybackManager& playbackManager;
    RhythmPatternManager& patternManager;
};

#endif /* PATTERN_EDIT_CONTROLLER */
