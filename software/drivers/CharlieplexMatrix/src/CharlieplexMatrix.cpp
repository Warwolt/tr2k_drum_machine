/*
****************************************************************************************************
* brief : Driver for charlieplexed LEDs
****************************************************************************************************
*/

#include "CharlieplexMatrix.h"
#include "GpioPin.h"

#ifdef UNIT_TESTING
#include "IGpioPinMock.h"
#include "traceprint.h"
#endif

template<typename IGpioPin>
CharlieplexMatrix<IGpioPin>::CharlieplexMatrix(u8 numPins, IGpioPin *pins, PinStatePair *stateLUT)
: numPins(numPins), numLeds(numPins * (numPins-1)), pins(pins), stateLUT(stateLUT)
{
    previousLedNum = numLeds - 1; // start at end of leds
}

template<typename IGpioPin>
u8 CharlieplexMatrix<IGpioPin>::getNumLeds() const
{
    return numLeds;
}

template<typename IGpioPin>
void CharlieplexMatrix<IGpioPin>::setLed(u8 ledNum)
{
    ledStates |= 0x1 << ledNum;
}

template<typename IGpioPin>
void CharlieplexMatrix<IGpioPin>::clearLed(u8 ledNum)
{
    ledStates &= ~(0x1 << ledNum);
}

template<typename IGpioPin>
void CharlieplexMatrix<IGpioPin>::toggleLed(u8 ledNum)
{
    ledStates ^= 0x1 << ledNum;
}

template<typename IGpioPin>
void CharlieplexMatrix<IGpioPin>::outputNextLed()
{
    IGpioPin& highPin = pins[stateLUT[currentLedNum].highPinNum];
    IGpioPin& lowPin = pins[stateLUT[currentLedNum].lowPinNum];

    /* Turn off previous led if it was on */
    bool previousLedActive = ledStates & (0x1 << previousLedNum);
    if(previousLedActive)
    {
        IGpioPin& previousHighPin = pins[stateLUT[previousLedNum].highPinNum];
        IGpioPin& previousLowPin = pins[stateLUT[previousLedNum].lowPinNum];
        previousHighPin.setDirection(DigitalInput);
        previousLowPin.setDirection(DigitalInput);
    }

    bool currentLedActive = ledStates & (0x1 << currentLedNum);
    if(currentLedActive)
    {
        highPin.setDirection(DigitalOutput);
        highPin.set();

        lowPin.setDirection(DigitalOutput);
        lowPin.clear();
    }
    else /* Led inactive */
    {
        highPin.setDirection(DigitalInput);
        lowPin.setDirection(DigitalInput);
    }

    previousLedNum = currentLedNum;
    currentLedNum = (currentLedNum + 1) % numLeds;
}

/* Explicit instantiation of template to avoid linking errors */
template class CharlieplexMatrix<GpioPin>;

#ifdef UNIT_TESTING
template class CharlieplexMatrix<GpioPinMock>;
#endif
