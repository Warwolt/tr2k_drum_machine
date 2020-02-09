/*
****************************************************************************************************
* brief : Class that handles delayed function calls
****************************************************************************************************
*/

#ifndef CALLBACK_SCHEDULER_H
#define CALLBACK_SCHEDULER_H

#include "MillisecondTimer.h"
#include "r2k/function.h"
#include "linuxtypes.h"

class CallbackScheduler
{
public:
    using CallbackFunction = r2k::function<void()>;
    CallbackScheduler(MillisecondTimer& timer);
    void scheduleCallback(const CallbackFunction& func, MillisecondTimer::milliseconds waitTime);
    void checkSchedule();

private:
    struct ScheduleInfo
    {
        CallbackFunction callback;
        MillisecondTimer::milliseconds startTime;
        MillisecondTimer::milliseconds waitTime;
    };

    void callAllDueCallbacks(MillisecondTimer::milliseconds currentTime);
    void cleanUpSchedule(MillisecondTimer::milliseconds currentTime);

    MillisecondTimer& timer;
    static constexpr u8 maxNumScheduledCallbacks = 32;
    u8 numScheduledCallbacks = 0;
    ScheduleInfo scheduledCallbacks[maxNumScheduledCallbacks];
};

#endif /* CALLBACK_SCHEDULER_H */
