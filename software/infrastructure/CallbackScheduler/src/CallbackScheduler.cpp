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
void CallbackScheduler::scheduleCallback(const CallbackFunction& func, milliseconds waitTime)
{
    if (numScheduledCallbacks < maxNumScheduledCallbacks)
    {
        ScheduleInfo& scheduling = scheduledCallbacks[numScheduledCallbacks];
        scheduling.callback = func;
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
            scheduling.callback();
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
