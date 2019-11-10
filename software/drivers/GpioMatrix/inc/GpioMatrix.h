/*
****************************************************************************************************
* brief : Driver for a matrix of gpio-pins, that can be used to connect many buttons, toggle
*         switches or other inputs with few used up gpio pins.
****************************************************************************************************
*/

#ifndef GPIO_MATRIX_H
#define GPIO_MATRIX_H

#include "linuxtypes.h"
#include "IGpioPin.h"
#include "MillisecondTimer.h"

template<typename IGpioPin>
class GpioMatrix
{
public:
    GpioMatrix(IGpioPin* columnPins, u8 numColumns, IGpioPin* rowPins, u8 numRows,
        MillisecondTimer& timer, MillisecondTimer::milliseconds debounceTime);
    u8 getNumElements();
    LogicState readElement(u8 elementNum);

private:
    struct ElementDebounceInformation
    {
        MillisecondTimer::milliseconds startTime;
        LogicState currentStableState;
        LogicState previousReadState;
    };

    LogicState readRawElementState(u8 elementNum);
    LogicState debounceElementState(LogicState state, u8 elementNum);

    u8 numColumns;
    u8 numRows;
    MillisecondTimer::milliseconds debounceTime;
    IGpioPin* columnPins;
    IGpioPin* rowPins;
    u8 previousColumnNum = 0;
    MillisecondTimer& timer;

    // I'm (Rasmus) currently only expecting to need one GpioMatrix in the
    // application and so I feel like I can get away with a fixed size array
    // for debounce information. This constant should be tweaked according to
    // the current needs in the application! I'm intentionally keeping the max
    // number of elements low to not needlesly waste any memory space.
    static const u8 MAX_NUM_ELEMENTS = 20;
    ElementDebounceInformation debounceInfo[MAX_NUM_ELEMENTS];
};

#endif /* GPIO_MATRIX_H */
