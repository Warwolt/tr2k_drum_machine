/*
****************************************************************************************************
* brief : Implementation of Led Group interface using CharlieplexMatrix driver.
****************************************************************************************************
*/

#ifndef CHARLIEPLEX_MAPPED_LED_GROUP_H
#define CHARLIEPLEX_MAPPED_LED_GROUP_H

#include "led_group.h"
#include "charlieplex_matrix.h"

template<typename IGpioPin>
class CharlieplexMappedLedGroup : public LedGroup
{
public:
    CharlieplexMappedLedGroup(u8 numLeds, CharlieplexMatrix<IGpioPin>& matrix);
    CharlieplexMappedLedGroup(u8 numLeds, u8 ledOffset, CharlieplexMatrix<IGpioPin>& matrix);
    u8 getNumLeds() const;
    void setLed(u8 ledNum);
    void clearLed(u8 ledNum);
    void toggleLed(u8 ledNum);
private:
    u8 numLeds;
    u8 ledOffset;
    CharlieplexMatrix<IGpioPin>& matrix;
};

#endif /* CHARLIEPLEX_MAPPED_LED_GROUP_H */
