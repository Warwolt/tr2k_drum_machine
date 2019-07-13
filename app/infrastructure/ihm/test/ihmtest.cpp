/*
****************************************************************************************************
* brief : Unit tests for InterruptHandlerManager (IHM)
****************************************************************************************************
*/

#include "gtest/gtest.h"
#include "traceprint.h"
#include "linuxtypes.h"
#include "ihm.h"
#include <unordered_map>

/* Import interrupt handlers, made possible by a mocked implementation of avr/interrupt.h */
extern void TIMER1_COMPA_vect (void);

static bool timer1CallbackCalled;

class TestInterruptHandlerManager : public ::testing::Test
{
public:
	InterruptHandlerManager ihm;

	void SetUp()
	{
		timer1CallbackCalled = false;
		avrmock::resetSeiWasCalled();
		avrmock::resetCliWasCalled();
	}
};

TEST_F(TestInterruptHandlerManager, Global_interrupts_can_be_enabled)
{
	ihm.enableInterruptsGlobally();
	EXPECT_TRUE(avrmock::seiWasCalled());
}

TEST_F(TestInterruptHandlerManager, Global_interrupts_can_be_disabled)
{
	ihm.disableInterruptsGlobally();
	EXPECT_TRUE(avrmock::cliWasCalled());
}

TEST_F(TestInterruptHandlerManager, Callback_can_be_attached_to_interrupt_TIMER1_COMPA)
{
	ihm.attachHandler([](){ timer1CallbackCalled = true; }, InterruptRequest::TIMER1_COMPA);

	TIMER1_COMPA_vect(); // call interrupt handler

	EXPECT_TRUE(timer1CallbackCalled) << ERROR_MSG_STR("Expected callback to have been called from "
		"TIMER1_COMPA interrupt handler, but it wasn't.");
}
