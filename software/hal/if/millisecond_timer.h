/*
****************************************************************************************************
* brief : Interface for timer that can track elapsed milliseconds
****************************************************************************************************
*/

#ifndef MILLISECOND_TIMER_H
#define MILLISECOND_TIMER_H

#include "linuxtypes.h"

class MillisecondTimer
{
public:
    using milliseconds = u32;

    virtual ~MillisecondTimer() {};
    virtual milliseconds getCurrentTime() = 0;
};

#endif /* MILLISECOND_TIMER_H */
