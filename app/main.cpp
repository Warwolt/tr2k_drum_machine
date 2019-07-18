#include "gpiopin.h"
#include "timer1.h"
#include "interrupts.h"
#include <avr/interrupt.h>
#include <avr/io.h>
#include "fixmath/fixmath.h"

static GpioPin ledPin = GpioPin(Pin5, PortB, DigitalOutput);
static Timer1 tim1;

void init();

int main()
{
	init();

	Fix16 num = fix16_from_int(5);

	while(1)
	{
		num++;
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
