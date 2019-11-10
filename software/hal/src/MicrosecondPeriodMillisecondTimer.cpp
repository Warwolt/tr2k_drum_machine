/*
****************************************************************************************************
* brief : Implementation of microsecond timer that supports HW timer using microsecond periods
*         using an 8-bit timer, in order to support triggering interrupts more often than 1 ms.
****************************************************************************************************
*/

#include "MicrosecondPeriodMillisecondTimer.h"
#include <math.h>

#ifdef UNIT_TESTING
#include "traceprint.h"
#endif

using milliseconds = MillisecondTimer::milliseconds;

static constexpr u32 clockFrequency = 16; // MHz

MicrosecondPeriodMillisecondTimer::MicrosecondPeriodMillisecondTimer(Timer8Bit& timer,
    microseconds microsecondPeriod) : microsecondPeriod(microsecondPeriod)
{
    u8 prescaler;
    Timer8Bit::PrescaleOption prescalerOption;

    if(microsecondPeriod > 128)
    {
        prescaler = 64;
        prescalerOption = Timer8Bit::PrescaleOption::_64;
    }
    else
    {
        prescaler = 8;
        prescalerOption = Timer8Bit::PrescaleOption::_8;
    }

    float clockPeriod = (prescaler / static_cast<float>(clockFrequency));
    u8 tickPeriod = round(static_cast<float>(microsecondPeriod) / clockPeriod);

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
