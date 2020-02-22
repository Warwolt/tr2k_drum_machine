/*
****************************************************************************************************
* brief : Manager class tracking the state of the rhythm pattern for all playback channels
****************************************************************************************************
*/

#ifndef RHYTHM_PATTERN_MANAGER
#define RHYTHM_PATTERN_MANAGER

#include "linuxtypes.h"
#include "r2k/vector.h"

struct RhythmPattern
{
    u16 state;
    u8 length;
};


class RhythmPatternManager
{
public:
    RhythmPatternManager();
    RhythmPattern getActivePattern();
    void toggleActivePatternStep(u8 stepNum);
private:
    u8 activePatternNum = 0;
    static constexpr u8 numRhythmPatterns = 16;
    r2k::vector<RhythmPattern, numRhythmPatterns> rhythmPatterns;
};

#endif /* RHYTHM_PATTERN_MANAGER */

