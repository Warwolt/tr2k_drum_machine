/*
****************************************************************************************************
* brief : Unit test for CallbackScheduler
****************************************************************************************************
*/

#include "gtest/gtest.h"
#include "traceprint.h"
#include "linuxtypes.h"
#include "CallbackScheduler.h"
#include "MillisecondTimerMock.h"

using ::testing::NiceMock;
using ::testing::Return;
using CallbackFunction0 = CallbackScheduler::CallbackFunction0;
using milliseconds = MillisecondTimer::milliseconds;

static constexpr u8 maxNumCallbacks = 2;
static bool callbackWasCalled[maxNumCallbacks];
static int capturedArgument = 0;

class CallbackSchedulerTest : public ::testing::Test
{
public:
    NiceMock<MillisecondTimerMock> mockTimer;
    CallbackScheduler scheduler = CallbackScheduler(mockTimer);

    void SetUp()
    {
        clearCallbacks();
    }

    void clearCallbacks()
    {
        capturedArgument = 0;
        for(int i = 0; i < maxNumCallbacks; i++)
        {
            callbackWasCalled[i] = false;
        }
    }
};

TEST_F(CallbackSchedulerTest, Callback_scheduled_with_no_delay_is_called_immediately)
{
    milliseconds waitTime = 0;
    CallbackScheduler::CallbackFunction0 func = [](){ callbackWasCalled[0] = true; };
    scheduler.scheduleCallback(func, waitTime);

    scheduler.checkSchedule();

    EXPECT_TRUE(callbackWasCalled[0]) << ERROR_MSG_STR("Expected callback to have been called");
}

TEST_F(CallbackSchedulerTest, Several_callbacks_can_be_scheduled_immediately)
{
    milliseconds waitTime = 0;
    CallbackScheduler::CallbackFunction0 func0 = [](){ callbackWasCalled[0] = true; };
    CallbackScheduler::CallbackFunction0 func1 = [](){ callbackWasCalled[1] = true; };
    scheduler.scheduleCallback(func0, waitTime);
    scheduler.scheduleCallback(func1, waitTime);

    scheduler.checkSchedule();

    EXPECT_TRUE(callbackWasCalled[0]) << ERROR_MSG_STR("Expected callback 0 to have been called");
    EXPECT_TRUE(callbackWasCalled[1]) << ERROR_MSG_STR("Expected callback 1 to have been called");
}

TEST_F(CallbackSchedulerTest, Callback_scheduled_with_delay_not_called_if_not_enough_time_passed)
{
    milliseconds waitTime = 100;
    CallbackScheduler::CallbackFunction0 func = [](){ callbackWasCalled[0] = true; };
    scheduler.scheduleCallback(func, waitTime);

    EXPECT_CALL(mockTimer, getCurrentTime()).WillOnce(Return(waitTime - 1));
    scheduler.checkSchedule();

    EXPECT_FALSE(callbackWasCalled[0]) << ERROR_MSG_STR("Expected callback to not have been called");
}

TEST_F(CallbackSchedulerTest, Callback_called_after_its_wait_time_has_elapsed)
{
    milliseconds waitTime = 100;
    CallbackScheduler::CallbackFunction0 func = [](){ callbackWasCalled[0] = true; };
    scheduler.scheduleCallback(func, waitTime);

    EXPECT_CALL(mockTimer, getCurrentTime()).WillOnce(Return(waitTime));
    scheduler.checkSchedule();

    EXPECT_TRUE(callbackWasCalled[0]) << ERROR_MSG_STR("Expected callback to have been called");
}

TEST_F(CallbackSchedulerTest, Callback_that_has_been_called_is_removed_from_the_schedule)
{
    milliseconds waitTime = 100;
    CallbackScheduler::CallbackFunction0 func = [](){ callbackWasCalled[0] = true; };
    scheduler.scheduleCallback(func, waitTime);

    /* First schedule check shuold call the callback and remove it */
    clearCallbacks();
    EXPECT_CALL(mockTimer, getCurrentTime()).WillOnce(Return(waitTime));
    scheduler.checkSchedule();
    EXPECT_TRUE(callbackWasCalled[0]) << ERROR_MSG_STR("Expected callback to have been called");

    /* Second schedule check should not call any functions */
    clearCallbacks();
    EXPECT_CALL(mockTimer, getCurrentTime()).WillOnce(Return(waitTime + 1));
    scheduler.checkSchedule();
    EXPECT_FALSE(callbackWasCalled[0]) << ERROR_MSG_STR("Expected callback to not have been called");
}

TEST_F(CallbackSchedulerTest, Multiple_callbacks_can_be_scheduled_for_different_times)
{
    milliseconds waitTime0 = 100;
    milliseconds waitTime1 = 250;
    CallbackScheduler::CallbackFunction0 func0 = [](){ callbackWasCalled[0] = true; };
    CallbackScheduler::CallbackFunction0 func1 = [](){ callbackWasCalled[1] = true; };
    scheduler.scheduleCallback(func1, waitTime1);
    scheduler.scheduleCallback(func0, waitTime0);

    /* First schedule check shuold call first callback only */
    clearCallbacks();
    EXPECT_CALL(mockTimer, getCurrentTime()).WillOnce(Return(waitTime0));
    scheduler.checkSchedule();
    EXPECT_TRUE(callbackWasCalled[0]) << ERROR_MSG_STR("Expected callback 0 to have been called");
    EXPECT_FALSE(callbackWasCalled[1]) << ERROR_MSG_STR("Expected callback 1 not to have been called");

    /* First schedule check shuold call first callback only */
    clearCallbacks();
    EXPECT_CALL(mockTimer, getCurrentTime()).WillOnce(Return(waitTime1));
    scheduler.checkSchedule();
    EXPECT_FALSE(callbackWasCalled[0]) << ERROR_MSG_STR("Expected callback 0 not to have been called");
    EXPECT_TRUE(callbackWasCalled[1]) << ERROR_MSG_STR("Expected callback 1 to have been called");
}

TEST_F(CallbackSchedulerTest, Callback_taking_one_parameter_called_with_recorded_argument)
{
    milliseconds waitTime = 150;
    u16 arg = 1234;
    CallbackScheduler::CallbackFunction1 func = [](u16 arg)
    {
        callbackWasCalled[0] = true;
        capturedArgument = arg;
    };
    scheduler.scheduleCallback(func, arg, waitTime);

    EXPECT_CALL(mockTimer, getCurrentTime()).WillOnce(Return(waitTime));
    scheduler.checkSchedule();

    EXPECT_TRUE(callbackWasCalled[0]) << ERROR_MSG_STR("Expected callback to have been called");
    EXPECT_EQ(arg, capturedArgument);
}
