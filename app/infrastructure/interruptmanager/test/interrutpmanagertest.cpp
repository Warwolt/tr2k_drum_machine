/*
****************************************************************************************************
* brief : Unit tests for Interrupf
****************************************************************************************************
*/

#include "gtest/gtest.h"
#include "traceprint.h"
#include "linuxtypes.h"
#include "interruptmanager.h"

/* Import interrupt handlers, made possible by a mocked implementation of avr/interrupt.h */
extern void TIMER1_COMPA_vect (void);
extern void SPI_STC_vect (void);

static bool handlerWasCalled;

using InterruptServiceRoutine = void(*)();

class TestInterruptManager : public ::testing::Test
{
public:
	InterruptManager interruptManager;

	void SetUp()
	{
		handlerWasCalled = false;
		avrmock::resetSeiWasCalled();
		avrmock::resetCliWasCalled();
	}

	/**
	 * Helper function for testing that handlers attached to an interrupt gets called from the
	 * corresponding service routine (which is the destination from the interrupt vector).
	 * @param interruptHandler  pointer to function to be called from service routine
	 * @param interruptRequest  enum specifying interrupt signal to attach handler to
	 * @param serviceRoutine    pointer to interrupt service routine belonging to interrupt
	 * @param interruptName     name of interrupt to print out in assertion error message
	 */
	void testInterruptHandlerCallsCallback(InterruptHandler interruptHandler,
		InterruptRequest interruptRequest, InterruptServiceRoutine serviceRoutine,
		std::string interruptName)
	{
		interruptManager.setHandlerForInterrupt(interruptHandler, interruptRequest);

		serviceRoutine();

		EXPECT_TRUE(handlerWasCalled) << ERROR_MSG_STR("Expected interrupt handler to have been "
			"called from " << interruptName << " interrupt service routine, but it wasn't.");
	}
};

TEST_F(TestInterruptManager, Global_interrupts_can_be_enabled)
{
	InterruptManager::enableInterruptsGlobally();
	EXPECT_TRUE(avrmock::seiWasCalled());
}

TEST_F(TestInterruptManager, Global_interrupts_can_be_disabled)
{
	InterruptManager::disableInterruptsGlobally();
	EXPECT_TRUE(avrmock::cliWasCalled());
}

TEST_F(TestInterruptManager, Callback_can_be_attached_to_interrupt_TIMER1_COMPA)
{
	InterruptHandler interruptHandler = [](){ handlerWasCalled = true; };
	InterruptRequest interruptRequest = InterruptRequest::TIMER1_COMPA;
	InterruptServiceRoutine interruptServiceRoutine = TIMER1_COMPA_vect;

	testInterruptHandlerCallsCallback(interruptHandler, interruptRequest, interruptServiceRoutine,
		"TIMER1_COMPA");
}

TEST_F(TestInterruptManager, Callback_can_be_attached_to_interrupt_SPI_STC)
{
	InterruptHandler interruptHandler = [](){ handlerWasCalled = true; };
	InterruptRequest interruptRequest = InterruptRequest::SPI_STC;
	InterruptServiceRoutine interruptServiceRoutine = SPI_STC_vect;

	testInterruptHandlerCallsCallback(interruptHandler, interruptRequest, interruptServiceRoutine,
		"SPI_STC");
}
