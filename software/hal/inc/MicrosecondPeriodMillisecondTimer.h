/*
****************************************************************************************************
* brief : Implementation of microsecond timer that supports HW timer using microsecond periods
*         using an 8-bit timer, in order to support triggering interrupts more often than 1 ms.
****************************************************************************************************
*/

#ifndef MICROSECOND_PERIOD_MILLISECOND_TIMER_H
#define MICROSECOND_PERIOD_MILLISECOND_TIMER_H

#include "linuxtypes.h"
#include "MillisecondTimer.h"
#include "Timer8Bit.h"

class MicrosecondPeriodMillisecondTimer : public MillisecondTimer
{
public:
    using microseconds = u16;
    MicrosecondPeriodMillisecondTimer(Timer8Bit& timer, microseconds period);
    void countPeriod();
    MillisecondTimer::milliseconds getCurrentTime();

private:
    microseconds microsecondPeriod;
    u16 elapsedMicroseconds = 0;
    MillisecondTimer::milliseconds elapsedMilliseconds = 0;
};

#endif /* MICROSECOND_PERIOD_MILLISECOND_TIMER_H */
