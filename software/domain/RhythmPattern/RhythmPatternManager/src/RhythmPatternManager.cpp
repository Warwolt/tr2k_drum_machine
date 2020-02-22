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

RhythmPattern RhythmPatternManager::getActivePattern()
{
    return rhythmPatterns[activePatternNum];
}

void RhythmPatternManager::toggleActivePatternStep(u8 stepNum)
{
    RhythmPattern& activePattern = rhythmPatterns[activePatternNum];
    activePattern.state ^= (0x1 << stepNum);
}
