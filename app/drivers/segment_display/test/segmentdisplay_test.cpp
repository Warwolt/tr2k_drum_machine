/*
****************************************************************************************************
* brief : Unit test for 74HC595 shift register based 4 digit 7-segment display driver
****************************************************************************************************
*/

#include "gtest/gtest.h"
#include "traceprint.h"
#include "segmentdisplay.h"
#include <map>

using digitValue = u8;
using segmentData = u8;

class TestSegmentDisplay74HC595 : public ::testing::Test
{
public:
	/* Spi mock */
	u8 pinDirectionReg = 0;
	u8 controlReg = 0;
	u8 statusReg = 0;
	u8 dataReg = 0;
	Spi spi = Spi(pinDirectionReg, controlReg, statusReg, dataReg);

	/* Code under test */
	SegmentDisplay74HC595 display = SegmentDisplay74HC595(spi);

	/* Look up table */
	std::map<digitValue, segmentData> segmentDataLookUpTable =
	{
		{0, 0xC0}, {1, 0xF9}, {2, 0xA4}, {3, 0xB0}, {4, 0x99},
		{5, 0x92}, {6, 0x82}, {7, 0xF8}, {8, 0x80}, {9, 0x90}
	};
};

TEST_F(TestSegmentDisplay74HC595, Digit_output_first_sends_segment_data_over_spi)
{
	display.setNumber(4);

	display.outputDigit(0);

	EXPECT_EQ(dataReg, segmentDataLookUpTable[4])
		<< ERROR_MSG_STR("Expected digit 0 segment data to have been sent over spi");
}

TEST_F(TestSegmentDisplay74HC595, Digit_output_then_sends_digit_data_over_spi)
{
	display.setNumber(4);
	display.outputDigit(0);

	spi.sendNextByteInBuffer();

	EXPECT_EQ(dataReg, 0x1 << 0)
		<< ERROR_MSG_STR("Expected digit data to have been sent over spi");
}

TEST_F(TestSegmentDisplay74HC595, Number_is_split_into_four_digits)
{
	display.setNumber(1957);

	display.outputDigit(0);
	EXPECT_EQ(dataReg, segmentDataLookUpTable[7]) << ERROR_MSG_STR("Expected digit 0 segment data");

	display.outputDigit(1);
	EXPECT_EQ(dataReg, segmentDataLookUpTable[5]) << ERROR_MSG_STR("Expected digit 1 segment data");

	display.outputDigit(2);
	EXPECT_EQ(dataReg, segmentDataLookUpTable[9]) << ERROR_MSG_STR("Expected digit 2 segment data");

	display.outputDigit(3);
	EXPECT_EQ(dataReg, segmentDataLookUpTable[1]) << ERROR_MSG_STR("Expected digit 3 segment data");
}

TEST_F(TestSegmentDisplay74HC595, Decimal_point_can_be_enabled)
{
	display.setNumber(14);
	display.enableDecimalPoint(1);

	display.outputDigit(1);

	EXPECT_EQ(dataReg, segmentDataLookUpTable[1] & ~(0x1 << 7))
		<< ERROR_MSG_STR("Expected segment data to have been sent with decimal point enabled");
}

TEST_F(TestSegmentDisplay74HC595, Decimal_point_can_be_disabled)
{
	display.setNumber(14);
	display.enableDecimalPoint(1);
	display.disableDecimalPoint(1);

	display.outputDigit(1);

	EXPECT_EQ(dataReg, segmentDataLookUpTable[1])
		<< ERROR_MSG_STR("Expected segment data to have been sent with decimal point enabled");
}
