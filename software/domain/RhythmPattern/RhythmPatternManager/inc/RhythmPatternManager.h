/*
****************************************************************************************************
* brief : Manager class tracking the state of the rhythm pattern for all playback channels
****************************************************************************************************
*/

#ifndef RHYTHM_PATTERN_MANAGER
#define RHYTHM_PATTERN_MANAGER

#include "linuxtypes.h"
#include "r2k/vector.h"

/**
 * @brief Represents an image of a rhythm pattern
 *
 * @param state  Each bit represents 1 for active step or 0 for inactive
 * @param length The number of steps in the pattern (0-16 steps)
 */
struct RhythmPattern
{
    u16 state;
    u8 length;
};

/**
 * @brief helper function for reading step state in pattern
 */
inline bool stepActiveInPattern(u8 stepNum, RhythmPattern pattern)
{
    return (pattern.state & (0x1 << stepNum));
}


class RhythmPatternManager
{
public:
    RhythmPatternManager();
    u8 getNumPatterns() const;
    RhythmPattern getPattern(const u8 patternNum);
    RhythmPattern getActivePattern();
    u8 getActivePatternNum() const;
    void selectActivePattern(const u8 patternNum);
    void toggleActivePatternStep(const u8 stepNum);
    void clearActivePatternStep(const u8 stepNum);
    void clearActivePattern();

private:
    u8 activePatternNum = 0;
    static constexpr u8 numRhythmPatterns = 16;
    r2k::vector<RhythmPattern, numRhythmPatterns> rhythmPatterns;
};

#endif /* RHYTHM_PATTERN_MANAGER */

