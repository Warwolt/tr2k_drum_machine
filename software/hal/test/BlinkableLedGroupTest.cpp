/*
****************************************************************************************************
* brief : Unit test for rotary encoder driver.
****************************************************************************************************
*/

#include "gtest/gtest.h"
#include "traceprint.h"
#include "linuxtypes.h"
#include "LedGroupMock.h"
#include "MillisecondTimerMock.h"
#include "BlinkableLedGroup.h"

using ::testing::NiceMock;
using ::testing::Return;
using milliseconds = MillisecondTimer::milliseconds;

class BlinkableLedGroupTest : public ::testing::Test
{
public:
    /* Dependencies */
    NiceMock<LedGroupMock> ledGroupMock;
    NiceMock<MillisecondTimerMock> timerMock;
    CallbackScheduler scheduler = CallbackScheduler(timerMock);

    /* Class under test */
    BlinkableLedGroup blinkableLeds = BlinkableLedGroup(ledGroupMock, scheduler);
};

/* Test decorations ------------------------------------------------------------------------------*/
TEST_F(BlinkableLedGroupTest, Blinking_inactive_led_first_sets_it_then_clears_it_via_scheduler)
{
    constexpr u8 ledNum = 2;
    constexpr MillisecondTimer::milliseconds startTime = 0;
    constexpr MillisecondTimer::milliseconds blinkPeriod = 100;

    /* Calling blinkLed() should cause a callback to be scheduled, which
     * requires us to make the millisecond timer inform the scheduler what the
     * current time is. */
    EXPECT_CALL(ledGroupMock, toggleLed(ledNum));
    EXPECT_CALL(timerMock, getCurrentTime()).WillOnce(Return(startTime));
    blinkableLeds.blinkLed(ledNum, blinkPeriod);

    /* When enough time has passed, the scheduler should now toggle the same
     * LED back to its initial state as soon as the schedule is checked. */
    EXPECT_CALL(timerMock, getCurrentTime()).WillOnce(Return(startTime + blinkPeriod));
    EXPECT_CALL(ledGroupMock, toggleLed(ledNum));
    scheduler.checkSchedule();
}


/* Test method forwarding ------------------------------------------------------------------------*/
TEST_F(BlinkableLedGroupTest, Forwards_set_led_to_decorated_led_group)
{
    EXPECT_CALL(ledGroupMock, setLed(5));
    blinkableLeds.setLed(5);
}

TEST_F(BlinkableLedGroupTest, Forwards_clear_led_to_decorated_led_group)
{
    EXPECT_CALL(ledGroupMock, clearLed(7));
    blinkableLeds.clearLed(7);
}

TEST_F(BlinkableLedGroupTest, Forwards_toggle_led_to_decorated_led_group)
{
    EXPECT_CALL(ledGroupMock, toggleLed(3));
    blinkableLeds.toggleLed(3);
}

TEST_F(BlinkableLedGroupTest, Forwards_get_num_leds_call_to_decorated_led_group)
{
    EXPECT_CALL(ledGroupMock, getNumLeds()).WillOnce(Return(13));
    EXPECT_EQ(blinkableLeds.getNumLeds(), 13);
}
