/*
****************************************************************************************************
* brief : Driver for a matrix of gpio-pins, that can be used to connect many buttons, toggle
*         switches or other inputs with few used up gpio pins.i
****************************************************************************************************
*/

#include "gpio_matrix.h"
#include "gpiopin.h"

#ifdef UNIT_TESTING
#include "igpiopin_mock.h"
#include "traceprint.h"
#endif

using milliseconds = MillisecondTimer::milliseconds;

template<typename IGpioPin>
GpioMatrix<IGpioPin>::GpioMatrix(IGpioPin* columnPins, u8 numColumns, IGpioPin* rowPins, u8 numRows,
    MillisecondTimer& timer, milliseconds debounceTime) : numColumns(numColumns),numRows(numRows),
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

    for(int i = 0; i < MAX_NUM_ELEMENTS; i++)
    {
        debounceInfo[i] = {startTime: 0, currentStableState: LogicState::Low,
            previousReadState: LogicState::Low};
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
    return debounceElementState(state, elementNum);
}

template<typename IGpioPin>
inline LogicState GpioMatrix<IGpioPin>::readRawElementState(u8 elementNum)
{
    u8 columnNum = elementNum % numColumns;
    u8 rowNum = elementNum / numColumns; // using truncation as floor()

    columnPins[previousColumnNum].setDirection(DigitalInput);
    columnPins[columnNum].setDirection(DigitalOutput);
    columnPins[columnNum].set();

    previousColumnNum = columnNum;
    return rowPins[rowNum].read();
}

template<typename IGpioPin>
inline LogicState GpioMatrix<IGpioPin>::debounceElementState(LogicState currentReadState, u8 elementNum)
{
    ElementDebounceInformation& currentElement = debounceInfo[elementNum];
    milliseconds currentTime = timer.getCurrentTime();
    bool stateIsStabilizing = (currentReadState == currentElement.previousReadState);
    bool enoughTimeElapsed = (currentTime - currentElement.startTime) >= debounceTime;

    if(stateIsStabilizing && enoughTimeElapsed)
    {
        currentElement.currentStableState = currentReadState;
    }
    else if(!stateIsStabilizing)
    {
        currentElement.startTime = currentTime;
    }

    currentElement.previousReadState = currentReadState;
    return currentElement.currentStableState;
}

/* Explicit instantiation of template to avoid linking errors */
template class GpioMatrix<GpioPin>;

#ifdef UNIT_TESTING
template class GpioMatrix<GpioPinMock>;
#endif
