/*
****************************************************************************************************
* brief : Class that handles delayed function calls
****************************************************************************************************
*/

#ifndef CALLBACK_SCHEDULER_H
#define CALLBACK_SCHEDULER_H

#include "MillisecondTimer.h"
#include "linuxtypes.h"

class CallbackScheduler
{
public:
    using CallbackFunction0 = void(*)();
    using CallbackFunction1 = void(*)(u16 x);

    CallbackScheduler(MillisecondTimer& timer);
    void scheduleCallback(CallbackFunction0 func, MillisecondTimer::milliseconds waitTime);
    void scheduleCallback(CallbackFunction1 func, u16 arg, MillisecondTimer::milliseconds waitTime);
    void checkSchedule();

private:
    struct ScheduleInfo
    {
        CallbackFunction0 func;
        bool has_arg;
        u16 arg;
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
