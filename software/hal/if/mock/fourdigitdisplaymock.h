/*
****************************************************************************************************
* brief : Mock implementation of FourDigitDisplay interface
****************************************************************************************************
*/

#ifndef FOUR_DIGIT_DISPLAY_MOCK_H
#define FOUR_DIGIT_DISPLAY_MOCK_H

#include "gmock/gmock.h"
#include "linuxtypes.h"
#include "fourdigitdisplay.h"

class FourDigitDisplayMock : public FourDigitDisplay
{
public:
	MOCK_METHOD1(setNumberToDisplay, void(u16 number));
	MOCK_METHOD1(enableDecimalPoint, void(u8 digit));
	MOCK_METHOD1(disableDecimalPoint, void(u8 digit));
};

#endif /* FOUR_DIGIT_DISPLAY_MOCK_H */

