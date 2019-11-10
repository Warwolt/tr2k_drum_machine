/*
****************************************************************************************************
* brief : Unit tests for DigitalTempoKnob
****************************************************************************************************
*/

#include "gtest/gtest.h"
#include "traceprint.h"
#include "linuxtypes.h"
#include "digital_tempo_knob.h"
#include "rotary_encoder.h"
#include "igpiopin_mock.h"

using ::testing::Return;

class DigitalTempoKnobTest : public ::testing::Test
{
public:
	GpioPinMock pinA, pinB;
	RotaryEncoder<GpioPinMock> encoder = RotaryEncoder<GpioPinMock>(pinA, pinB);
	DigitalTempoKnob<GpioPinMock> knob = DigitalTempoKnob<GpioPinMock>(encoder);

	void increaseNumMockKnobRotations()
	{
		EXPECT_CALL(pinA, read()).WillOnce(Return(LogicState::Low));
		EXPECT_CALL(pinB, read()).WillOnce(Return(LogicState::Low));
		encoder.handleEdge();
	}

	void decreaseNumMockKnobRotations()
	{
		EXPECT_CALL(pinA, read()).WillOnce(Return(LogicState::Low));
		EXPECT_CALL(pinB, read()).WillOnce(Return(LogicState::High));
		encoder.handleEdge();
	}
};

TEST_F(DigitalTempoKnobTest, Initial_reference_bpm_value_is_120)
{
	EXPECT_EQ(BeatsPerMinute(120), knob.read());
}

TEST_F(DigitalTempoKnobTest, Reference_bpm_value_can_be_set)
{
	knob.setReferenceTempo(60);
	EXPECT_EQ(BeatsPerMinute(60), knob.read());
}

TEST_F(DigitalTempoKnobTest, Bpm_increases_when_encoder_rotations_increases)
{
	increaseNumMockKnobRotations();
	increaseNumMockKnobRotations();
	EXPECT_EQ(BeatsPerMinute(120 + 2), knob.read());
}

TEST_F(DigitalTempoKnobTest, Bpm_decreases_when_encoder_rotations_decreases)
{
	decreaseNumMockKnobRotations();
	decreaseNumMockKnobRotations();
	EXPECT_EQ(BeatsPerMinute(120 - 2), knob.read());
}
