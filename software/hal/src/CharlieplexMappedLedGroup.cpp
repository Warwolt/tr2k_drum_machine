/*
****************************************************************************************************
* brief : Implementation of Led Group interface using CharlieplexMatrix driver.
****************************************************************************************************
*/

#include "CharlieplexMappedLedGroup.h"
#include "GpioPin.h"

#ifdef UNIT_TESTING
#include "IGpioPinMock.h"
#include "traceprint.h"
#endif

template<typename IGpioPin>
CharlieplexMappedLedGroup<IGpioPin>::CharlieplexMappedLedGroup(u8 numLeds,
    CharlieplexMatrix<IGpioPin>& matrix) : numLeds(numLeds), ledOffset(0), matrix(matrix)
{

}

template<typename IGpioPin>
CharlieplexMappedLedGroup<IGpioPin>::CharlieplexMappedLedGroup(u8 numLeds, u8 ledOffset,
    CharlieplexMatrix<IGpioPin>& matrix) : numLeds(numLeds), ledOffset(ledOffset), matrix(matrix)
{

}

template<typename IGpioPin>
u8 CharlieplexMappedLedGroup<IGpioPin>::getNumLeds() const
{
    return numLeds;
}

template<typename IGpioPin>
void CharlieplexMappedLedGroup<IGpioPin>::setLed(u8 ledNum)
{
    matrix.setLed(ledNum + ledOffset);
}

template<typename IGpioPin>
void CharlieplexMappedLedGroup<IGpioPin>::clearLed(u8 ledNum)
{
    matrix.clearLed(ledNum + ledOffset);
}

template<typename IGpioPin>
void CharlieplexMappedLedGroup<IGpioPin>::toggleLed(u8 ledNum)
{
    matrix.toggleLed(ledNum + ledOffset);
}

/* Explicit instantiation of template to avoid linking errors */
template class CharlieplexMappedLedGroup<GpioPin>;

#ifdef UNIT_TESTING
template class CharlieplexMappedLedGroup<GpioPinMock>;
#endif
