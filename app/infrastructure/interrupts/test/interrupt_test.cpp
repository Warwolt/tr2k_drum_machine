/*
****************************************************************************************************
* brief : Unit tests for Interrupf
****************************************************************************************************
*/

#include "gtest/gtest.h"
#include "traceprint.h"
#include "linuxtypes.h"
#include "interrupts.h"

/* Import interrupt handlers, made possible by a mocked implementation of avr/interrupt.h */
extern void TIMER0_COMPA_vect (void);
extern void TIMER1_COMPA_vect (void);
extern void SPI_STC_vect (void);

static bool handlerWasCalled;

using InterruptServiceRoutine = void(*)();

class TestInterrupts : public ::testing::Test
{
public:

	void SetUp()
	{
		handlerWasCalled = false;
		AvrMock::resetSeiWasCalled();
		AvrMock::resetCliWasCalled();
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
		Interrupts::setHandlerForInterrupt(interruptHandler, interruptRequest);

		serviceRoutine();

		EXPECT_TRUE(handlerWasCalled) << ERROR_MSG_STR("Expected interrupt handler to have been "
			"called from " << interruptName << " interrupt service routine, but it wasn't.");
	}
};

TEST_F(TestInterrupts, Global_interrupts_can_be_enabled)
{
	Interrupts::enableInterruptsGlobally();
	EXPECT_TRUE(AvrMock::seiWasCalled());
}

TEST_F(TestInterrupts, Global_interrupts_can_be_disabled)
{
	Interrupts::disableInterruptsGlobally();
	EXPECT_TRUE(AvrMock::cliWasCalled());
}

TEST_F(TestInterrupts, Callback_can_be_attached_to_interrupt_TIMER0_COMPA)
{
	InterruptHandler interruptHandler = [](){ handlerWasCalled = true; };
	InterruptRequest interruptRequest = InterruptRequest::Timer0CompareMatch;
	InterruptServiceRoutine interruptServiceRoutine = TIMER0_COMPA_vect;

	testInterruptHandlerCallsCallback(interruptHandler, interruptRequest, interruptServiceRoutine,
		"Timer0CompareMatch");
}

TEST_F(TestInterrupts, Callback_can_be_attached_to_interrupt_TIMER1_COMPA)
{
	InterruptHandler interruptHandler = [](){ handlerWasCalled = true; };
	InterruptRequest interruptRequest = InterruptRequest::Timer1CompareMatch;
	InterruptServiceRoutine interruptServiceRoutine = TIMER1_COMPA_vect;

	testInterruptHandlerCallsCallback(interruptHandler, interruptRequest, interruptServiceRoutine,
		"Timer1CompareMatch");
}

TEST_F(TestInterrupts, Callback_can_be_attached_to_interrupt_SPI_STC)
{
	InterruptHandler interruptHandler = [](){ handlerWasCalled = true; };
	InterruptRequest interruptRequest = InterruptRequest::SpiTransferComplete;
	InterruptServiceRoutine interruptServiceRoutine = SPI_STC_vect;

	testInterruptHandlerCallsCallback(interruptHandler, interruptRequest, interruptServiceRoutine,
		"SpiTransferComplete");
}
