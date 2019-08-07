/*
****************************************************************************************************
* brief : Unit test for "timer1" 16-bit timer driver code
****************************************************************************************************
*/

#include "gtest/gtest.h"
#include "traceprint.h"
#include "linuxtypes.h"
#include "timer1.h"


class TestTimer1 : public ::testing::Test
{
public:
	u8 controlRegister = 0;
	u8 maskRegister = 0;
	u16 compareRegister = 0;
	u16 counterRegister = 0;
	Timer1 timer = Timer1(controlRegister, maskRegister, compareRegister, counterRegister);

	void testPrescaler(Timer16Bit::PrescaleOption prescaler, u8 clockSelectBits)
	{
		controlRegister = 0xC0;
		u8 expectation = controlRegister | clockSelectBits;

		timer.setPrescaler(prescaler);
		timer.start();

		EXPECT_EQ(expectation, controlRegister);
	}
};

/* Interrupt enable tests ------------------------------------------------------------------------*/
TEST_F(TestTimer1, Enabling_interrupts_sets_mode_to_clear_timer_on_compare)
{
	controlRegister = 0xA0;
	u8 expectation = controlRegister | 0x1 << WGM12;
	timer.enablePeriodicInterrupts();
	EXPECT_EQ(expectation, controlRegister);
}

TEST_F(TestTimer1, Enabling_interrupt_sets_compare_match_interrupt_flag)
{
	maskRegister = 0xB0;
	u8 expectation = maskRegister | 0x1 << OCIE1A;
	timer.enablePeriodicInterrupts();
	EXPECT_EQ(expectation , maskRegister);
}

/* Prescaler tests -------------------------------------------------------------------------------*/
TEST_F(TestTimer1, Prescaler_set_to_1_is_used_when_starting_timer)
{
	testPrescaler(Timer16Bit::PrescaleOption::_1, (0x1 << CS10));
}

TEST_F(TestTimer1, Prescaler_set_to_8_is_used_when_starting_timer)
{
	testPrescaler(Timer16Bit::PrescaleOption::_8, (0x2 << CS10));
}

TEST_F(TestTimer1, Prescaler_set_to_64_is_used_when_starting_timer)
{
	testPrescaler(Timer16Bit::PrescaleOption::_64, (0x3 << CS10));
}

TEST_F(TestTimer1, Prescaler_set_to_256_is_used_when_starting_timer)
{
	testPrescaler(Timer16Bit::PrescaleOption::_256, (0x4 << CS10));
}

TEST_F(TestTimer1, Prescaler_set_to_1024_is_used_when_starting_timer)
{
	testPrescaler(Timer16Bit::PrescaleOption::_1024, (0x5 << CS10));
}

/* Timer control tests ---------------------------------------------------------------------------*/
TEST_F(TestTimer1, Stop_sets_clock_source_to_none)
{
	controlRegister = 0xAA;
	u8 expectation = controlRegister & ~(0x7);
	timer.stop();
	EXPECT_EQ(expectation, controlRegister);
}

TEST_F(TestTimer1, Clear_resets_counter_register)
{
	counterRegister = 0xAA;
	timer.clear();
	EXPECT_EQ(0, counterRegister);
}

TEST_F(TestTimer1, Setting_period_writes_to_output_compare_register)
{
	timer.setPeriod(500);
	EXPECT_EQ(499, compareRegister);
}
