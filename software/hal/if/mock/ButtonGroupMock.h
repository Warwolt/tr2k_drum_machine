/*
****************************************************************************************************
* brief : Mock implementation of ButtonGroup interface
****************************************************************************************************
*/

#ifndef BUTTON_GROUP_MOCK_H
#define BUTTON_GROUP_MOCK_H

#include "gmock/gmock.h"
#include "ButtonGroup.h"
#include "linuxtypes.h"

class ButtonGroupMock : public ButtonGroup
{
public:
	MOCK_METHOD1(buttonPressedNow, bool(u8 buttonNum));
	MOCK_METHOD1(buttonReleasedNow, bool(u8 buttonNum));
	MOCK_METHOD1(buttonIsUp, bool(u8 buttonNum));
	MOCK_METHOD1(buttonIsDown, bool(u8 buttonNum));
	MOCK_CONST_METHOD0(getNumButtons, u8(void));
};

#endif /* BUTTON_GROUP_MOCK_H */
