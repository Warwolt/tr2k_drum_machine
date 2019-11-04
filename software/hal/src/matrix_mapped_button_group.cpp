/*
****************************************************************************************************
* brief : Implementation of ButtonGroup using GpioInputMatrix
****************************************************************************************************
*/

#include "matrix_mapped_button_group.h"
#include "linuxtypes.h"
#include "gpiopin.h"

#ifdef UNIT_TESTING
#include "igpiopin_mock.h"
#endif

template<typename IGpioPin>
MatrixMappedButtonGroup<IGpioPin>::MatrixMappedButtonGroup(GpioMatrix<IGpioPin>& matrix,
    u8 numButtons, u8 elementOffset) : matrix(matrix), numButtons(numButtons), elementOffset(elementOffset)
{

}

template<typename IGpioPin>
bool MatrixMappedButtonGroup<IGpioPin>::buttonPressedNow(u8 buttonNum)
{
    LogicState currentReadState = matrix.readElement(buttonNum + elementOffset);
    bool previousStateLow = (previousReadState[buttonNum] == LogicState::Low);
    bool currentStateHigh = (currentReadState == LogicState::High);
    previousReadState[buttonNum] = currentReadState;
    return (previousStateLow and currentStateHigh);
}

template<typename IGpioPin>
bool MatrixMappedButtonGroup<IGpioPin>::buttonReleasedNow(u8 buttonNum)
{
    LogicState currentReadState = matrix.readElement(buttonNum + elementOffset);
    bool previousStateHigh = (previousReadState[buttonNum] == LogicState::High);
    bool currentStateLow = (currentReadState == LogicState::Low);
    previousReadState[buttonNum] = currentReadState;
    return (previousStateHigh and currentStateLow);
}

template<typename IGpioPin>
bool MatrixMappedButtonGroup<IGpioPin>::buttonIsUp(u8 buttonNum)
{
    LogicState readState = matrix.readElement(buttonNum + elementOffset);
    return (readState == LogicState::Low);
}

template<typename IGpioPin>
bool MatrixMappedButtonGroup<IGpioPin>::buttonIsDown(u8 buttonNum)
{
    LogicState readState = matrix.readElement(buttonNum + elementOffset);
    return (readState == LogicState::High);
}

/* Explicit instantiation of template to avoid linking errors */
template class MatrixMappedButtonGroup<GpioPin>;

#ifdef UNIT_TESTING
template class MatrixMappedButtonGroup<GpioPinMock>;
#endif
