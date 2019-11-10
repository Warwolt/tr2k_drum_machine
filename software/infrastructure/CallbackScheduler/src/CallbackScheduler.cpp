/*
****************************************************************************************************
* brief : Class that handles delayed function calls
****************************************************************************************************
*/

#include "CallbackScheduler.h"
#include "traceprint.h"

using milliseconds = MillisecondTimer::milliseconds;

CallbackScheduler::CallbackScheduler(MillisecondTimer& timer) : timer(timer)
{

}

/**
 * @brief Add a callback function to be called after some time
 *
 * @param func      callback function taking zero arguments to call
 * @param waitTime  Number of milliseconds to wait before calling
 */
void CallbackScheduler::scheduleCallback(CallbackFunction0 func, milliseconds waitTime)
{
    if(!func)
    {
        return;
    }

    if(numScheduledCallbacks < maxNumScheduledCallbacks)
    {
        ScheduleInfo& scheduling = scheduledCallbacks[numScheduledCallbacks];
        scheduling.func = func;
        scheduling.has_arg = false;
        scheduling.startTime = timer.getCurrentTime();
        scheduling.waitTime = waitTime;
        numScheduledCallbacks++;
    }
}

/**
 * @brief Add a callback function to be called after some time
 *
 * @param func      callback function taking zero arguments to call
 * @param arg       argument to pass to callback function
 * @param waitTime  Number of milliseconds to wait before calling
 */
void CallbackScheduler::scheduleCallback(CallbackFunction1 func, u16 arg, milliseconds waitTime)
{
    if(!func)
    {
        return;
    }

    if(numScheduledCallbacks < maxNumScheduledCallbacks)
    {
        ScheduleInfo& scheduling = scheduledCallbacks[numScheduledCallbacks];
        scheduling.func = reinterpret_cast<CallbackFunction0>(func); // only store address
        scheduling.has_arg = true;
        scheduling.arg = arg;
        scheduling.startTime = timer.getCurrentTime();
        scheduling.waitTime = waitTime;
        numScheduledCallbacks++;
    }
}

/**
 * @brief Check the schedule for any callbacks that should be called
 */
void CallbackScheduler::checkSchedule()
{
    milliseconds currentTime = timer.getCurrentTime();
    callAllDueCallbacks(currentTime);
    cleanUpSchedule(currentTime);
}

/**
 * @brief Call every callback that have waited enough time
 */
void CallbackScheduler::callAllDueCallbacks(milliseconds currentTime)
{
    for(int i = 0; i < numScheduledCallbacks; i++)
    {
        ScheduleInfo scheduling = scheduledCallbacks[i];
        if(currentTime - scheduling.startTime >= scheduling.waitTime)
        {
            if(scheduling.has_arg)
            {
                CallbackFunction1 func = reinterpret_cast<CallbackFunction1>(scheduling.func);
                func(scheduling.arg);
            }
            else
            {
                CallbackFunction0& func0 = scheduling.func;
                func0();
            }

        }
    }
}

/**
 * @brief Remove any callbacks that have been executed from the schedule
 */
void CallbackScheduler::cleanUpSchedule(milliseconds currentTime)
{
    for(int i = 0; i < numScheduledCallbacks; i++)
    {
        ScheduleInfo currentScheduling = scheduledCallbacks[i];
        milliseconds elapsedTime = currentTime - currentScheduling.startTime;
        if(elapsedTime >= currentScheduling.waitTime)
        {
            /* Remove function by replacing it with last function in list */
            scheduledCallbacks[i] = scheduledCallbacks[numScheduledCallbacks - 1];
            numScheduledCallbacks--;
        }
    }
}
