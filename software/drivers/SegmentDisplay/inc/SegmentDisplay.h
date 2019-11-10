/*
****************************************************************************************************
* brief : Implementation of 74HC595 shift register based 4 digit 7-segment display
****************************************************************************************************
*/

#ifndef SEGMENT_DISPLAY_H
#define SEGMENT_DISPLAY_H

#include "FourDigitDisplay.h"
#include "linuxtypes.h"
#include "Spi.h"

class SegmentDisplay74HC595 : public FourDigitDisplay
{
public:
	SegmentDisplay74HC595(Spi& spi);
	void setNumberToDisplay(u16 number);
	void enableDecimalPoint(u8 digit);
	void disableDecimalPoint(u8 digit);
	void outputDigit(u8 digit);

private:
	Spi& spi;
	r2k::vector<u8, 4> numberDigits;
	r2k::vector<bool, 4> decimalPointEnabled;
};

#endif /* SEGMENT_DISPLAY_H */
