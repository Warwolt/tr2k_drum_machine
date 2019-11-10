/*
****************************************************************************************************
* brief : Unit test for "timer0" 16-bit timer driver code
****************************************************************************************************
*/

#include "gtest/gtest.h"
#include "traceprint.h"
#include "linuxtypes.h"
#include "Timer0.h"

class TestTimer0 : public ::testing::Test
{
public:
	u8 controlRegisterA = 0;
	u8 controlRegisterB = 0;
	u8 maskRegister = 0;
	u8 compareRegister = 0;
	u8 counterRegister = 0;
	Timer0 timer = Timer0(controlRegisterA, controlRegisterB, maskRegister,
		compareRegister, counterRegister);

	void SetUp()
	{
	}

	void testPrescaler(Timer8Bit::PrescaleOption prescaler, u8 clockSelectBits)
	{
		controlRegisterB = 0xC0;
		u8 expectation = controlRegisterB | clockSelectBits;

		timer.setPrescaler(prescaler);
		timer.start();

		EXPECT_EQ(expectation, controlRegisterB);
	}
};

/* Interrupt enable tests ------------------------------------------------------------------------*/
TEST_F(TestTimer0, Enabling_interrupts_sets_mode_to_clear_timer_on_compare)
{
	controlRegisterA = 0xA0;
	u8 expectation = controlRegisterA | 0x1 << WGM01;
	timer.enablePeriodicInterrupts();
	EXPECT_EQ(expectation, controlRegisterA);
}

TEST_F(TestTimer0, Enabling_interrupt_sets_compare_match_interrupt_flag)
{
	maskRegister = 0xB0;
	u8 expectation = maskRegister | 0x1 << OCIE0A;
	timer.enablePeriodicInterrupts();
	EXPECT_EQ(expectation , maskRegister);
}

/* Prescaler tests -------------------------------------------------------------------------------*/
TEST_F(TestTimer0, Prescaler_set_to_1_is_used_when_starting_timer)
{
	testPrescaler(Timer8Bit::PrescaleOption::_1, (0x1 << CS00));
}

TEST_F(TestTimer0, Prescaler_set_to_8_is_used_when_starting_timer)
{
	testPrescaler(Timer8Bit::PrescaleOption::_8, (0x2 << CS00));
}

TEST_F(TestTimer0, Prescaler_set_to_64_is_used_when_starting_timer)
{
	testPrescaler(Timer8Bit::PrescaleOption::_64, (0x3 << CS00));
}

TEST_F(TestTimer0, Prescaler_set_to_256_is_used_when_starting_timer)
{
	testPrescaler(Timer8Bit::PrescaleOption::_256, (0x4 << CS00));
}

TEST_F(TestTimer0, Prescaler_set_to_1024_is_used_when_starting_timer)
{
	testPrescaler(Timer8Bit::PrescaleOption::_1024, (0x5 << CS00));
}

/* Timer control tests ---------------------------------------------------------------------------*/
TEST_F(TestTimer0, Stop_sets_clock_source_to_none)
{
	controlRegisterB = 0xAA;
	u8 expectation = controlRegisterB & ~(0x7);
	timer.stop();
	EXPECT_EQ(expectation, controlRegisterB);
}

TEST_F(TestTimer0, Clear_resets_counter_register)
{
	counterRegister = 0xAA;
	timer.clear();
	EXPECT_EQ(0, counterRegister);
}

TEST_F(TestTimer0, Setting_period_writes_to_output_compare_register)
{
	timer.setPeriod(200);
	EXPECT_EQ(199, compareRegister);
}
