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
 * @param func      CallbackFunction to call
 * @param waitTime  Number of milliseconds to wait before calling
 */
void CallbackScheduler::scheduleCallback(CallbackFunction func, milliseconds waitTime)
{
    if(numScheduledCallbacks < maxNumScheduledCallbacks)
    {
        scheduledCallbacks[numScheduledCallbacks].func = func;
        scheduledCallbacks[numScheduledCallbacks].startTime = timer.getCurrentTime();
        scheduledCallbacks[numScheduledCallbacks].waitTime = waitTime;
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

void CallbackScheduler::callAllDueCallbacks(milliseconds currentTime)
{
    for(int i = 0; i < numScheduledCallbacks; i++)
    {
        ScheduleInfo currentScheduling = scheduledCallbacks[i];
        if(currentTime - currentScheduling.startTime >= currentScheduling.waitTime)
        {
            if(currentScheduling.func != NULL)
            {
                currentScheduling.func(); // Call the scheduled callback function
            }
        }
    }
}

void CallbackScheduler::cleanUpSchedule(milliseconds currentTime)
{
    for(int i = 0; i < numScheduledCallbacks; i++)
    {
        ScheduleInfo currentScheduling = scheduledCallbacks[i];
        if(currentTime - currentScheduling.startTime >= currentScheduling.waitTime)
        {
            /* Remove function by replacing it with last function in list */
            scheduledCallbacks[i] = scheduledCallbacks[numScheduledCallbacks - 1];
            numScheduledCallbacks--;
        }
    }
}
