/*
****************************************************************************************************
* brief : Driver for charlieplexed LEDs
****************************************************************************************************
*/

#ifndef CHARLIEPLEX_MATRIX_H
#define CHARLIEPLEX_MATRIX_H

#include "linuxtypes.h"

struct PinStatePair
{
    u8 highPinNum;
    u8 lowPinNum;
};

template<typename IGpioPin>
class CharlieplexMatrix
{
public:
    CharlieplexMatrix(u8 numPins, IGpioPin* pins, PinStatePair* stateLUT);
    u8 getNumLeds() const;
    void setLed(u8 ledNum);
    void clearLed(u8 ledNum);
    void toggleLed(u8 ledNum);
    void outputNextLed();

private:
    void outputLed(u8 ledNum);

    const u8 numPins;
    const u8 numLeds;
    u8 currentLedNum = 0;
    u8 previousLedNum;
    u16 ledStates = 0;
    IGpioPin* pins;
    PinStatePair* stateLUT;
};

#endif /* CHARLIEPLEX_MATRIX_H */
