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

TEST_F(RhythmPatternManagerTest, Steps_in_active_pattern_can_be_toggled)
{
    patternManager.toggleActivePatternStep(0);
    patternManager.toggleActivePatternStep(2);
    RhythmPattern activePattern = patternManager.getActivePattern();
    EXPECT_EQ(activePattern.state, 0x5);
}
