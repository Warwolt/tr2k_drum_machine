/*
****************************************************************************************************
* brief : Unit test for MicrosecondPeriodMillisecondTimer
****************************************************************************************************
*/

#include "gtest/gtest.h"
#include "traceprint.h"
#include "linuxtypes.h"
#include "igpiopin_mock.h"
#include "microsecond_period_millisecond_timer.h"
#include "timer8bitmock.h"
#include <cmath>

using milliseconds = MillisecondTimer::milliseconds;
using microseconds = MicrosecondPeriodMillisecondTimer::microseconds;
using usecMillisecondTimer = MicrosecondPeriodMillisecondTimer;
using ::testing::NiceMock;

class MicrosecondPeriodMillisecondTimerTest : public ::testing::Test
{
public:
    const float clockFrequency = 16e6; // Hz
    const microseconds microsecondPeriod = 53;

    NiceMock<Timer8BitMock> mockTimer;
    usecMillisecondTimer usecTimer = usecMillisecondTimer(mockTimer, microsecondPeriod);
};

TEST_F(MicrosecondPeriodMillisecondTimerTest, Period_up_to_128_usec_uses_prescaler_8)
{
    const microseconds period = 64;
    const float prescaler = 8;
    const u8 expectedTickPeriod = std::round(period * 1000.0 / (float)(clockFrequency / prescaler));
    EXPECT_CALL(mockTimer, setPrescaler(Timer8Bit::PrescaleOption::_8));
    EXPECT_CALL(mockTimer, setPeriod(expectedTickPeriod));
    usecMillisecondTimer(mockTimer, period);
}

TEST_F(MicrosecondPeriodMillisecondTimerTest, Period_over_128_usec_uses_prescaler_64)
{
    const microseconds period = 130;
    const float prescaler = 64;
    const u8 expectedTickPeriod = std::round(period * 1000.0 / (float)(clockFrequency / prescaler));
    EXPECT_CALL(mockTimer, setPrescaler(Timer8Bit::PrescaleOption::_64));
    EXPECT_CALL(mockTimer, setPeriod(expectedTickPeriod));
    usecMillisecondTimer(mockTimer, period);
}

TEST_F(MicrosecondPeriodMillisecondTimerTest, Initial_number_of_elapsed_milliseconds_is_zero)
{
    EXPECT_EQ(0u, usecTimer.getCurrentTime());
}

TEST_F(MicrosecondPeriodMillisecondTimerTest, One_millisecond_elapsed_after_enough_usec_periods)
{
    u16 numPeriods = std::ceil(1.0 * 1000.0 / microsecondPeriod);
    for(int i = 0; i < numPeriods; i++)
    {
        usecTimer.countPeriod();
    }
    EXPECT_EQ(1u, usecTimer.getCurrentTime());
}

TEST_F(MicrosecondPeriodMillisecondTimerTest, Two_hundred_millisecond_elapsed_after_enough_usec_periods)
{
    u16 numPeriods = std::ceil(200.0 * 1000.0 / microsecondPeriod);
    for(int i = 0; i < numPeriods; i++)
    {
        usecTimer.countPeriod();
    }
    EXPECT_EQ(200u, usecTimer.getCurrentTime());
}

TEST_F(MicrosecondPeriodMillisecondTimerTest, Hundred_thousand_millisecond_elapsed_after_enough_usec_periods)
{
    u32 numPeriods = std::ceil(100e3 * 1000.0 / microsecondPeriod);
    for(u32 i = 0; i < numPeriods; i++)
    {
        usecTimer.countPeriod();
    }
    EXPECT_EQ(100e3, usecTimer.getCurrentTime());
}
