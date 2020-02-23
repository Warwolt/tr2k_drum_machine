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
    void selectActivePattern(u8 patternNum);
    u8 getActivePatternNum() const;
    RhythmPattern getActivePattern();
    void toggleActivePatternStep(u8 stepNum);
    void clearActivePatternStep(u8 stepNum);
    void clearActivePattern();

    RhythmPattern getPattern(u8 patternNum);
    bool playbackIsOngoing();
    u8 getPlaybackPosition();

private:
    RhythmPlaybackManager& playbackManager;
    RhythmPatternManager& patternManager;
};

#endif /* PATTERN_EDIT_CONTROLLER */
