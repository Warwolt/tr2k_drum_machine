/*
****************************************************************************************************
* brief : Interface for a display for 4 digits
****************************************************************************************************
*/

#ifndef FOUR_DIGIT_DISPLAY_H
#define FOUR_DIGIT_DISPLAY_H

#include "linuxtypes.h"

class FourDigitDisplay
{
public:
	virtual ~FourDigitDisplay() {}
	virtual void setNumber(u16 number) = 0;
	virtual void enableDecimalPoint(u8 digit) = 0;
	virtual void disableDecimalPoint(u8 digit) = 0;
};

#endif /* FOUR_DIGIT_DISPLAY_H */
