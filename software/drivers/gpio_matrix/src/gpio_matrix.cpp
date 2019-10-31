/*
****************************************************************************************************
* brief : Driver for a matrix of gpio-pins, that can be used to connect many buttons, toggle
*         switches or other inputs with few used up gpio pins.
****************************************************************************************************
*/

#include "gpio_matrix.h"
#include "gpiopin.h"

#ifdef UNIT_TESTING
#include "igpiopin_mock.h"
#include "traceprint.h"
#endif

template<typename IGpioPin>
GpioMatrix<IGpioPin>::GpioMatrix(IGpioPin* columnPins, u8 numColumns, IGpioPin* rowPins, u8 numRows,
    MillisecondTimer& timer, milliseconds_t debounceTime) : numColumns(numColumns),numRows(numRows),
    debounceTime(debounceTime), columnPins(columnPins), rowPins(rowPins), timer(timer)
{
    for(int i = 0; i < numRows; i++)
    {
        rowPins[i].setDirection(DigitalInput);
    }

    for(int i = 0; i < numColumns; i++)
    {
        columnPins[i].setDirection(DigitalInput);
    }
}

template<typename IGpioPin>
u8 GpioMatrix<IGpioPin>::getNumElements()
{
    return numColumns * numRows;
}

template<typename IGpioPin>
LogicState GpioMatrix<IGpioPin>::readElement(u8 elementNum)
{
    LogicState state = readRawElementState(elementNum);
    return debounceElementState(state);
}

template<typename IGpioPin>
inline LogicState GpioMatrix<IGpioPin>::readRawElementState(u8 elementNum)
{
    u8 columnNum = elementNum % numColumns;
    u8 rowNum = elementNum / numColumns; // using truncation as floor()

    columnPins[columnNum].setDirection(DigitalOutput);
    columnPins[columnNum].set();

    return rowPins[rowNum].read();
}

template<typename IGpioPin>
inline LogicState GpioMatrix<IGpioPin>::debounceElementState(LogicState currentReadState)
{
    milliseconds_t currentTime = timer.getCurrentTime();
    bool stateIsStabilizing = (currentReadState == previousReadState);
    bool enoughTimeElapsed = (currentTime - startTime) >= debounceTime;

    if(stateIsStabilizing && enoughTimeElapsed)
    {
        currentStableState = currentReadState;
    }
    else if(!stateIsStabilizing)
    {
        startTime = currentTime;
    }

    previousReadState = currentReadState;
    return currentStableState;
}

/* Explicit instantiation of template to avoid linking errors */
template class GpioMatrix<GpioPin>;

#ifdef UNIT_TESTING
template class GpioMatrix<GpioPinMock>;
#endif
