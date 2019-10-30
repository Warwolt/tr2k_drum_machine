/*
****************************************************************************************************
* brief : Interface for classes managing a group of individually controllable LEDs
****************************************************************************************************
*/

#include "linuxtypes.h"

class LedGroup
{
public:
    virtual ~LedGroup() {};
    virtual void setLed(u8 ledNum) = 0;
    virtual void clearLed(u8 ledNum) = 0;
    virtual void toggleLed(u8 ledNum) = 0;
    virtual u8 getNumLeds() const = 0;
};
