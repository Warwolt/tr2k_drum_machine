/*
****************************************************************************************************
* brief : Decoration of Led Group that adds support for blinking leds with callback scheduler.
****************************************************************************************************
*/

#include "BlinkableLedGroup.h"
using milliseconds = MillisecondTimer::milliseconds;

BlinkableLedGroup::BlinkableLedGroup(LedGroup& ledGroup, CallbackScheduler& scheduler)
: ledGroup(ledGroup), scheduler(scheduler)
{

}

/**
 * @brief Blink a led
 *
 * Blinks a led by toggling its state once immediately, and then again via a
 * callback called from the callback scheduler. In order to make this work,
 * the callback scheduler _must_ be regularly polled!
 *
 * @param ledNum       Which led to blink
 * @param blinkPeriod  Duration of blink
 */
void BlinkableLedGroup::blinkLed(u8 ledNum, milliseconds blinkPeriod)
{
    ledGroup.toggleLed(ledNum);
    scheduler.scheduleCallback([=](){ledGroup.toggleLed(ledNum);}, blinkPeriod);
}

/**
 * @brief Forward setLed to decorated LedGroup
 */
void BlinkableLedGroup::setLed(u8 ledNum)
{
    ledGroup.setLed(ledNum);
}

/**
 * @brief Forward clearLed to decorated LedGroup
 */
void BlinkableLedGroup::clearLed(u8 ledNum)
{
    ledGroup.clearLed(ledNum);
}

/**
 * @brief Forward toggleLed to decorated LedGroup
 */
void BlinkableLedGroup::toggleLed(u8 ledNum)
{
    ledGroup.toggleLed(ledNum);
}

/**
 * @brief Forward getNumLeds to decorated LedGroup
 */
u8 BlinkableLedGroup::getNumLeds() const
{
    return ledGroup.getNumLeds();
}

