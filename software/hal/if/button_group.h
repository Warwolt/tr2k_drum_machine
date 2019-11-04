/*
****************************************************************************************************
* brief : Interface for classes managing a group of individually readable buttons
****************************************************************************************************
*/

#ifndef BUTTON_GROUP_H
#define BUTTON_GROUP_H

#include "linuxtypes.h"

class ButtonGroup
{
public:
    virtual ~ButtonGroup() {};
    virtual bool buttonPressedNow(u8 buttonNum) = 0;
    virtual bool buttonReleasedNow(u8 buttonNum) = 0;
    virtual bool buttonIsUp(u8 buttonNum) = 0;
    virtual bool buttonIsDown(u8 buttonNum) = 0;
};

#endif /* BUTTON_GROUP_H */
