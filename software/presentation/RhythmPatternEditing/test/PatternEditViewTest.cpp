/*
****************************************************************************************************
* brief : Unit tests for PatternEditView
****************************************************************************************************
*/

#include "gtest/gtest.h"
#include "traceprint.h"
#include "linuxtypes.h"
#include "PatternEditView.h"
#include "TempoTimerMock.h"

using ::testing::Return;
using ::testing::NiceMock;

class PatternEditViewTest : public ::testing::Test
{
public:
    PatternEditView editView;
};

TEST_F(PatternEditViewTest, DISABLED_Clears_all_step_leds_if_active_pattern_is_empty_during_update)
{
    FAIL() << ERROR_MSG_STR("Implement RhythmPatternManager::getActivePatternState()!");
}
