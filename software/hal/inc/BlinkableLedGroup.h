/*
****************************************************************************************************
* brief : Decoration of Led Group that adds support for blinking leds with callback scheduler.
****************************************************************************************************
*/

#ifndef BLINKABLE_LED_GROUP_H
#define BLINKABLE_LED_GROUP_H

#include "LedGroup.h"
#include "CallbackScheduler.h"

class BlinkableLedGroup : public LedGroup
{
public:
    BlinkableLedGroup(LedGroup& ledGroup, CallbackScheduler& scheduler);
    void blinkLed(u8 ledNum, MillisecondTimer::milliseconds blinkPeriod);
    void setLed(u8 ledNum);
    void clearLed(u8 ledNum);
    void toggleLed(u8 ledNum);
    u8 getNumLeds() const;

private:
    LedGroup& ledGroup;
    CallbackScheduler& scheduler;
};

#endif /* BLINKABLE_LED_GROUP_H */

