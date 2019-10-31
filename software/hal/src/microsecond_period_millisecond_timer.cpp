/*
****************************************************************************************************
* brief : Implementation of microsecond timer that supports HW timer using microsecond periods
*         using an 8-bit timer, in order to support triggering interrupts more often than 1 ms.
****************************************************************************************************
*/

#include "microsecond_period_millisecond_timer.h"
#include <math.h>

#ifdef UNIT_TESTING
#include "traceprint.h"
#endif

using milliseconds = MillisecondTimer::milliseconds;

static constexpr u32 clockFrequency = 16e6;

MicrosecondPeriodMillisecondTimer::MicrosecondPeriodMillisecondTimer(Timer8Bit& timer,
    microseconds period) : microsecondPeriod(period)
{
    u8 prescaler;
    Timer8Bit::PrescaleOption prescalerOption;

    if(period > 128)
    {
        prescaler = 64;
        prescalerOption = Timer8Bit::PrescaleOption::_64;
    }
    else
    {
        prescaler = 8;
        prescalerOption = Timer8Bit::PrescaleOption::_8;
    }

    u8 tickPeriod = round(period * 1000 / (clockFrequency / static_cast<float>(prescaler)));

    timer.setPeriod(tickPeriod);
    timer.setPrescaler(prescalerOption);
}

void MicrosecondPeriodMillisecondTimer::countPeriod()
{
    elapsedMicroseconds += microsecondPeriod;
    if(elapsedMicroseconds >= 1000)
    {
        elapsedMicroseconds -= 1000;
        elapsedMilliseconds += 1;
    }
}

milliseconds MicrosecondPeriodMillisecondTimer::getCurrentTime()
{
    return elapsedMilliseconds;
}
