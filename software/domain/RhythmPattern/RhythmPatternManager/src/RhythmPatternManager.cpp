/*
****************************************************************************************************
* brief : Manager class tracking the state of the rhythm pattern for all playback channels
****************************************************************************************************
*/

#include "RhythmPatternManager.h"

RhythmPatternManager::RhythmPatternManager()
{
    for (size_t i = 0; i < rhythmPatterns.capacity(); i++)
    {
        rhythmPatterns.push_back({.state = 0, .length = 16});
    }
}

/**
 * @brief Select which pattern is active i.e. currently edited
 *
 * @param patternNum  Channel id corresponding to pattern to chose
 */
void RhythmPatternManager::selectActivePattern(const u8 patternNum)
{
    activePatternNum = patternNum;
}

RhythmPattern RhythmPatternManager::getActivePattern()
{
    return rhythmPatterns[activePatternNum];
}

/**
 * @brief Return the number of patterns in manager
 */
u8 RhythmPatternManager::getNumPatterns() const
{
    return rhythmPatterns.capacity();
}

/**
 * @brief Get an image of a pattern in the manager
 */
RhythmPattern RhythmPatternManager::getPattern(const u8 patternNum)
{
    return rhythmPatterns[patternNum];
}

void RhythmPatternManager::toggleActivePatternStep(const u8 stepNum)
{
    RhythmPattern& activePattern = rhythmPatterns[activePatternNum];
    activePattern.state ^= (0x1 << stepNum);
}

void RhythmPatternManager::clearActivePattern()
{
    RhythmPattern& activePattern = rhythmPatterns[activePatternNum];
    activePattern.state = 0;
}
