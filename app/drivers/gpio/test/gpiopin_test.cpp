/*
****************************************************************************************************
* brief : Unit test for gpio pin driver
****************************************************************************************************
*/

#include "gtest/gtest.h"
#include "traceprint.h"
#include "gpiopin.h"
#include <vector>

class TestGpioPin : public ::testing::Test
{
public:

	GpioPin testPin = GpioPin(PIN_0, PORT_B);
	std::vector<GpioPin> testPins = {
		GpioPin(PIN_0, PORT_B), GpioPin(PIN_1, PORT_B), GpioPin(PIN_2, PORT_B) };

	uint8_t inputRegister = 0;
	uint8_t outputRegister = 0;
	uint8_t dataDirectionRegister = 0;

	void SetUp()
	{
		testPin.setInputRegister(inputRegister);
		testPin.setOutputRegister(outputRegister);
		testPin.setDataDirectionRegister(dataDirectionRegister);
		testPin.setDirection(DIGITAL_OUTPUT); // use output as default

		for(auto &pin : testPins)
		{
			pin.setInputRegister(inputRegister);
			pin.setOutputRegister(outputRegister);
			pin.setDataDirectionRegister(dataDirectionRegister);
			pin.setDirection(DIGITAL_OUTPUT);
		}
	}
};

/* Set data direction tests ----------------------------------------------------------------------*/
TEST_F(TestGpioPin, Configuring_pin_as_output_should_set_correct_bit_in_datadir_reg)
{
	dataDirectionRegister = 0;

	testPin.setDirection(DIGITAL_OUTPUT);

	EXPECT_EQ(0x1, dataDirectionRegister) << "Expected datadir register bit to be set";
}

TEST_F(TestGpioPin, Configuring_pin_as_input_should_clear_correct_bit_in_datadir_reg)
{
	dataDirectionRegister = 0xFF;

	testPin.setDirection(DIGITAL_INPUT);

	EXPECT_EQ(0xFE, dataDirectionRegister) << "Expected datadir register bit to be cleared";
}

TEST_F(TestGpioPin, Configuring_pin_as_input_should_clear_correct_bit_in_output_reg)
{
	outputRegister = 0xFF;

	testPin.setDirection(DIGITAL_INPUT);

	EXPECT_EQ(0xFE, outputRegister) << "Expected output register bit to be cleared";
}

/* Pin output tests ------------------------------------------------------------------------------*/
TEST_F(TestGpioPin, Setting_pin_sets_correct_bit_in_output_register)
{
	outputRegister = 0;

	testPin.set();

	EXPECT_EQ(0x1, outputRegister) << "Expected output register bit to be set";
}

TEST_F(TestGpioPin, Setting_several_pins_sets_correct_bits_in_output_register)
{
	outputRegister = 0;

	for(auto &pin : testPins)
		pin.set();

	EXPECT_EQ(0x7, outputRegister) << "Expected output register bits to be set";
}

TEST_F(TestGpioPin, Setting_input_pin_does_nothing_to_output_register)
{
	outputRegister = 0;

	testPin.setDirection(DIGITAL_INPUT);
	testPin.set();

	EXPECT_EQ(0, outputRegister) << "Expected output register bit to be cleared";
}

TEST_F(TestGpioPin, Clearing_pin_resets_correct_bit_in_output_register)
{
	outputRegister = 0xFF;

	testPin.clear();

	EXPECT_EQ(0xFE, outputRegister) << "Expected output register bit to be cleared";
}

TEST_F(TestGpioPin, Clearing_several_pins_resets_correct_bits_in_output_register)
{
	outputRegister = 0xFF;

	for(auto &pin : testPins)
		pin.clear();

	EXPECT_EQ(0xF8, outputRegister) << "Expected output register bits to be cleared";
}

TEST_F(TestGpioPin, Write_pin_method_can_set_a_pin)
{
	outputRegister = 0;

	testPin.write(LOGIC_HIGH);

	EXPECT_EQ(0x1, outputRegister) << "Expected output register bit to be set";
}

TEST_F(TestGpioPin, Write_pin_method_cannot_set_an_input_pin)
{
	outputRegister = 0;

	testPin.setDirection(DIGITAL_INPUT);
	testPin.write(LOGIC_HIGH);

	EXPECT_EQ(0x0, outputRegister) << "Expected output register bit to be cleared";
}

TEST_F(TestGpioPin, Write_pin_method_can_clear_a_pin)
{
	outputRegister = 0xFF;

	testPin.write(LOGIC_LOW);

	EXPECT_EQ(0xFE, outputRegister) << "Expected output register bit to be cleared";
}

TEST_F(TestGpioPin, Toggling_initially_low_pin_sets_it)
{
	outputRegister = 0;

	testPin.toggle();

	EXPECT_EQ(0x1, outputRegister) << "Expected output register bit to be set";
}

TEST_F(TestGpioPin, Toggling_initially_high_pin_clears_it)
{
	outputRegister = 0xFF;

	testPin.toggle();

	EXPECT_EQ(0xFE, outputRegister) << "Expected output register bit to be cleared";
}

TEST_F(TestGpioPin, Toggling_input_pin_does_nothing)
{
	outputRegister = 0;

	testPin.setDirection(DIGITAL_INPUT);
	testPin.toggle();

	EXPECT_EQ(0x0, outputRegister) << "Expected output register bit to be cleared";
}

/* Pin input tests -------------------------------------------------------------------------------*/
TEST_F(TestGpioPin, Reading_pin_returns_low_if_input_register_is_empty)
{
	inputRegister = 0;

	LogicState actualLogicState = testPin.read();

	EXPECT_EQ(LOGIC_LOW, actualLogicState) << "Expected pin state to have been read low";
}

TEST_F(TestGpioPin, Reading_pin_returns_low_if_wrong_bit_in_input_register_is_set)
{
	inputRegister = 0x2; // test pin should check bit 0

	LogicState actualLogicState = testPin.read();

	EXPECT_EQ(LOGIC_LOW, actualLogicState) << "Expected pin state to have been read low";
}

TEST_F(TestGpioPin, Reading_pin_returns_high_if_input_register_has_set_bit)
{
	inputRegister = 0x1;

	LogicState actualLogicState = testPin.read();

	EXPECT_EQ(LOGIC_HIGH, actualLogicState) << "Expected pin state to have been read high";
}
