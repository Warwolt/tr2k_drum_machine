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

void PatternEditController::selectActivePattern(u8 patternNum)
{
    patternManager.selectActivePattern(patternNum);
}

u8 PatternEditController::getActivePatternNum() const
{
    return patternManager.getActivePatternNum();
}

RhythmPattern PatternEditController::getPattern(u8 patternNum)
{
    return patternManager.getPattern(patternNum);
}

void PatternEditController::toggleActivePatternStep(u8 stepNum)
{
    patternManager.toggleActivePatternStep(stepNum);
}

void PatternEditController::clearActivePatternStep(u8 stepNum)
{
    patternManager.clearActivePatternStep(stepNum);
}

void PatternEditController::clearActivePattern()
{
    patternManager.clearActivePattern();
}

u8 PatternEditController::getPlaybackPosition()
{
    return playbackManager.getPlaybackPosition();
}
