/*
****************************************************************************************************
* brief : Implementation of 74HC595 shift register based 4 digit 7-segment display
****************************************************************************************************
*/

#include "segmentdisplay.h"
#include "traceprint.h"

static constexpr u8 segmentDataLookupTable[10]
{
	// 0,    1,    2,    3,    4,    5,    6,    7,    8,    9
	0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90
};

SegmentDisplay74HC595::SegmentDisplay74HC595(Spi& spi) : spi(spi)
{
	decimalPointEnabled = {false, false, false, false};
}

void SegmentDisplay74HC595::setNumberToDisplay(u16 number)
{
	numberDigits =
	{
		static_cast<u8>(number % 10),
		static_cast<u8>(number/10 % 10),
		static_cast<u8>(number/100 % 10),
		static_cast<u8>(number/1000 % 10),
	};
}

void SegmentDisplay74HC595::enableDecimalPoint(u8 digit)
{
	decimalPointEnabled[digit] = true;
}

void SegmentDisplay74HC595::disableDecimalPoint(u8 digit)
{
	decimalPointEnabled[digit] = false;
}

void SegmentDisplay74HC595::outputDigit(u8 digit)
{
	u8 segmentData = segmentDataLookupTable[numberDigits[digit]];

	if(decimalPointEnabled[digit])
	{
		segmentData &= ~(0x1 << 7);
	};

	r2k::vector<u8, 2> txBuffer = {segmentData, static_cast<u8>(0x1 << digit)};
	spi.setTxBuffer(txBuffer);

	spi.sendNextByteInBuffer();
}
