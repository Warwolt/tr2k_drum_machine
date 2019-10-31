/*
****************************************************************************************************
* brief : Interface for timer that can track elapsed milliseconds
****************************************************************************************************
*/

#ifndef MILLISECOND_TIMER_H
#define MILLISECOND_TIMER_H

#include "linuxtypes.h"

using milliseconds_t = u32;

class MillisecondTimer
{
public:
    virtual ~MillisecondTimer() {};
    virtual milliseconds_t getCurrentTime() = 0;
};

#endif /* MILLISECOND_TIMER_H */
