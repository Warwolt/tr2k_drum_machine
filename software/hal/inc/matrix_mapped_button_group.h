/*
****************************************************************************************************
* brief : Implementation of ButtonGroup using GpioInputMatrix
****************************************************************************************************
*/

#ifndef MATRIX_MAPPED_BUTTON_GROUP_H
#define MATRIX_MAPPED_BUTTON_GROUP_H

#include "linuxtypes.h"
#include "gpio_matrix.h"
#include "button_group.h"

template<typename IGpioPin>
class MatrixMappedButtonGroup : public ButtonGroup
{
public:
    MatrixMappedButtonGroup(GpioMatrix<IGpioPin>& matrix, u8 numButtons, u8 elementOffset);
    bool buttonPressedNow(u8 buttonNum);
    bool buttonReleasedNow(u8 buttonNum);
    bool buttonIsUp(u8 buttonNum);
    bool buttonIsDown(u8 buttonNum);

private:
    GpioMatrix<IGpioPin>& matrix;
    static const u8 MAX_NUM_BUTTONS = 20; // change this as needed
    LogicState previousReadState[MAX_NUM_BUTTONS];
    u8 numButtons;
    u8 elementOffset;
};

#endif /* MATRIX_MAPPED_BUTTON_GROUP_H */
