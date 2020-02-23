/*
****************************************************************************************************
* @brief  MVC Controller for rhythm pattern editing
****************************************************************************************************
*/

#include "PatternEditController.h"

PatternEditController::PatternEditController(RhythmPlaybackManager& playbackManager,
        RhythmPatternManager& patternManager)
    : playbackManager(playbackManager), patternManager(patternManager) {}

bool PatternEditController::playbackIsOngoing()
{
    return playbackManager.playbackIsOngoing();
}

RhythmPattern PatternEditController::getActivePattern()
{
    return patternManager.getActivePattern();
}

RhythmPattern PatternEditController::getPattern(u8 patternNum)
{
    return patternManager.getPattern(patternNum);
}

u8 PatternEditController::getPlaybackPosition()
{
    return playbackManager.getPlaybackPosition();
}
