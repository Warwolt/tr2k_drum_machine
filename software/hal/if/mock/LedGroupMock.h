/*
****************************************************************************************************
* brief : Mock implementation of LedGroup interface
****************************************************************************************************
*/

#ifndef LED_GROUP_MOCK_H
#define LED_GROUP_MOCK_H

#include "gmock/gmock.h"
#include "LedGroup.h"
#include "linuxtypes.h"

class LedGroupMock : public LedGroup
{
public:
	MOCK_METHOD1(setLed, void(u8 ledNum));
	MOCK_METHOD1(clearLed, void(u8 ledNum));
	MOCK_METHOD1(toggleLed, void(u8 ledNum));
	MOCK_CONST_METHOD0(getNumLeds, u8(void));
};

#endif /* LED_GROUP_MOCK_H */
