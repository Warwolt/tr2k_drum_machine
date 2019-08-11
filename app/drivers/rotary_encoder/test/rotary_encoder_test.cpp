/*
****************************************************************************************************
* brief : Unit test for rotary encoder driver.
****************************************************************************************************
*/

#include "gtest/gtest.h"
#include "traceprint.h"
#include "linuxtypes.h"
#include "rotary_encoder.h"
#include "igpiopin_mock.h"

using ::testing::Return;

class RotaryEncoderTest : public ::testing::Test
{
public:
	GpioPinMock pinA;
	GpioPinMock pinB;
	RotaryEncoder<GpioPinMock> encoder = RotaryEncoder<GpioPinMock>(pinA, pinB);
};

TEST_F(RotaryEncoderTest, Initial_number_of_rotations_is_zero)
{
	EXPECT_EQ(0, encoder.getNumRotations());
}

TEST_F(RotaryEncoderTest, If_pin_A_high_when_handling_edge_then_does_nothing)
{
	EXPECT_CALL(pinA, read()).WillOnce(Return(LogicState::High));

	encoder.handleEdge();

	EXPECT_EQ(0, encoder.getNumRotations());
}

TEST_F(RotaryEncoderTest, If_pin_A_low_and_pin_B_low_when_handling_edge_then_increment_rotations)
{
	EXPECT_CALL(pinA, read()).WillOnce(Return(LogicState::Low));
	EXPECT_CALL(pinB, read()).WillOnce(Return(LogicState::Low));

	encoder.handleEdge();

	EXPECT_EQ(1, encoder.getNumRotations());
}

TEST_F(RotaryEncoderTest, If_pin_A_low_and_pin_B_high_when_handling_edge_then_decrement_rotations)
{
	EXPECT_CALL(pinA, read()).WillOnce(Return(LogicState::Low));
	EXPECT_CALL(pinB, read()).WillOnce(Return(LogicState::High));

	encoder.handleEdge();

	EXPECT_EQ(-1, encoder.getNumRotations());
}

TEST_F(RotaryEncoderTest, Number_of_rotations_can_be_incremented_repeatedly)
{
	EXPECT_CALL(pinA, read()).WillRepeatedly(Return(LogicState::Low));
	EXPECT_CALL(pinB, read()).WillRepeatedly(Return(LogicState::Low));

	encoder.handleEdge();
	encoder.handleEdge();
	encoder.handleEdge();

	EXPECT_EQ(3, encoder.getNumRotations());
}

TEST_F(RotaryEncoderTest, Number_of_rotations_can_be_decremented_repeatedly)
{
	EXPECT_CALL(pinA, read()).WillRepeatedly(Return(LogicState::Low));
	EXPECT_CALL(pinB, read()).WillRepeatedly(Return(LogicState::High));

	encoder.handleEdge();
	encoder.handleEdge();
	encoder.handleEdge();

	EXPECT_EQ(-3, encoder.getNumRotations());
}
