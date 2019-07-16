#include "gpiopin.h"
#include "timer1.h"
#include "interrupts.h"
#include <avr/interrupt.h>
#include <avr/io.h>

static GpioPin ledPin = GpioPin(PIN_5, PORT_B, DIGITAL_OUTPUT);
static Timer1 tim1;

void init();

int main()
{
	init();

	while(1)
	{
		//
	}
}

void init()
{
	Interrupts::enableInterruptsGlobally();

	InterruptHandler interruptHandler = []{ ledPin.toggle(); };
	InterruptRequest interruptRequest = InterruptRequest::Timer1CompareMatch;
	Interrupts::setHandlerForInterrupt(interruptHandler, interruptRequest);

	tim1.enablePeriodicInterrupts();
	tim1.setPrescaler(Timer16Bit::PrescaleOption::_1);
	tim1.setPeriod(160);
	tim1.start();
}
