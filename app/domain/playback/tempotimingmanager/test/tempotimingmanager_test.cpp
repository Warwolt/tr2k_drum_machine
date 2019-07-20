/*
****************************************************************************************************
* brief : Unit tests for TempoTimingManager
****************************************************************************************************
*/

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "traceprint.h"
#include "tempotimingmanager.h"
#include "tempotimermock.h"

using testing::NiceMock;

class TestTempoTimingManager : public ::testing::Test
{
public:
	NiceMock<TempoTimerMock> mockTimer;
};

TEST_F(TestTempoTimingManager, Hello)
{
	FAIL() << ERROR_MSG_STR("Hello");
}
