/*
****************************************************************************************************
* brief : Driver for a matrix of gpio-pins, that can be used to connect many buttons, toggle
*         switches or other inputs with few used up gpio pins.
****************************************************************************************************
*/

#ifndef GPIO_MATRIX_H
#define GPIO_MATRIX_H

#include "linuxtypes.h"
#include "igpiopin.h"
#include "millisecond_timer.h"

template<typename IGpioPin>
class GpioMatrix
{
public:
    GpioMatrix(IGpioPin* columnPins, u8 numColumns, IGpioPin* rowPins, u8 numRows,
        MillisecondTimer& timer, milliseconds_t debounceTime);
    u8 getNumElements();
    LogicState readElement(u8 elementNum);

private:
    LogicState readRawElementState(u8 elementNum);
    LogicState debounceElementState(LogicState state);

    u8 numColumns;
    u8 numRows;
    milliseconds_t debounceTime;
    IGpioPin* columnPins;
    IGpioPin* rowPins;
    MillisecondTimer& timer;

    milliseconds_t startTime = 0;
    LogicState previousReadState = LogicState::Low;
    LogicState currentStableState = LogicState::Low;
};

#endif /* GPIO_MATRIX_H */
