/*
****************************************************************************************************
* brief : Unit tests for InterrupManager
****************************************************************************************************
*/

#include "gtest/gtest.h"
#include "traceprint.h"
#include "linuxtypes.h"
#include "interruptmanager.h"
#include <unordered_map>

/* Import interrupt handlers, made possible by a mocked implementation of avr/interrupt.h */
extern void TIMER1_COMPA_vect (void);
extern void SPI_STC_vect (void);

static bool timer1CallbackCalled;
static bool spiStcCallbackCalled;

class TestInterruptHandlerManager : public ::testing::Test
{
public:
	InterruptManager manager;

	void SetUp()
	{
		timer1CallbackCalled = false;
		spiStcCallbackCalled = false;

		avrmock::resetSeiWasCalled();
		avrmock::resetCliWasCalled();
	}
};

TEST_F(TestInterruptHandlerManager, Global_interrupts_can_be_enabled)
{
	manager.enableInterruptsGlobally();
	EXPECT_TRUE(avrmock::seiWasCalled());
}

TEST_F(TestInterruptHandlerManager, Global_interrupts_can_be_disabled)
{
	manager.disableInterruptsGlobally();
	EXPECT_TRUE(avrmock::cliWasCalled());
}

TEST_F(TestInterruptHandlerManager, Callback_can_be_attached_to_interrupt_TIMER1_COMPA)
{
	manager.setHandler(InterruptRequest::TIMER1_COMPA, [](){ timer1CallbackCalled = true; });

	TIMER1_COMPA_vect(); // call interrupt handler

	EXPECT_TRUE(timer1CallbackCalled) << ERROR_MSG_STR("Expected callback to have been called from "
		"TIMER1_COMPA interrupt handler, but it wasn't.");
}

TEST_F(TestInterruptHandlerManager, Callback_can_be_attached_to_interrupt_SPI_STC)
{
	manager.setHandler(InterruptRequest::SPI_STC, [](){ spiStcCallbackCalled = true; });

	SPI_STC_vect(); // call interrupt handler

	EXPECT_TRUE(spiStcCallbackCalled) << ERROR_MSG_STR("Expected callback to have been called from "
		"SPI_STC interrupt handler, but it wasn't.");
}
