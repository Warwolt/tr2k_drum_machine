/*
****************************************************************************************************
* brief : Unit tests for PatternEditView
****************************************************************************************************
*/

#include "gtest/gtest.h"
#include "traceprint.h"
#include "linuxtypes.h"
#include "RhythmPatternManager.h"
#include "TempoTimerMock.h"

using ::testing::Return;
using ::testing::NiceMock;

class RhythmPatternManagerTest : public ::testing::Test
{
public:
RhythmPatternManager patternManager;
};

TEST_F(RhythmPatternManagerTest, Active_pattern_is_initially_empty)
{
    RhythmPattern activePattern = patternManager.getActivePattern();
    EXPECT_EQ(activePattern.state, 0);
}

TEST_F(RhythmPatternManagerTest, All_patterns_have_initial_length_16)
{
    for (int i = 0; i < patternManager.getNumPatterns(); i++)
    {
        RhythmPattern currentPattern = patternManager.getPattern(i);
        EXPECT_EQ(currentPattern.length, 16);
    }
}

TEST_F(RhythmPatternManagerTest, Steps_in_active_pattern_can_be_toggled)
{
    patternManager.toggleActivePatternStep(0);
    patternManager.toggleActivePatternStep(2);
    RhythmPattern activePattern = patternManager.getActivePattern();
    EXPECT_EQ(activePattern.state, 0x5);
}

TEST_F(RhythmPatternManagerTest, Active_pattern_can_be_cleared)
{
    patternManager.toggleActivePatternStep(0); // make sure there's something to clear
    patternManager.clearActivePattern();
    EXPECT_EQ(patternManager.getActivePattern().state, 0);
}

TEST_F(RhythmPatternManagerTest, Active_pattern_can_be_selected)
{
    /* Edit pattern 1 */
    patternManager.selectActivePattern(1);
    patternManager.toggleActivePatternStep(0);
    patternManager.toggleActivePatternStep(1);
    EXPECT_EQ(patternManager.getActivePattern().state, 0x3);

    /* Pattern 0 should be unchanged */
    patternManager.selectActivePattern(0);
    EXPECT_EQ(patternManager.getActivePattern().state, 0);
}
