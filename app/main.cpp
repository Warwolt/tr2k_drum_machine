#include "gpiopin.h"
#include "timer1.h"
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

static GpioPin ledPin = GpioPin(PIN_5, PORT_B, DIGITAL_OUTPUT);
static Timer1 tim1;

void init();

ISR(TIMER1_COMPA_vect)
{
	ledPin.toggle();
}

int main()
{
	init();

	while(1)
	{
	}
}

void init()
{
	// enable global interrupts
	sei();

	// setup timer1
	tim1.enablePeriodicInterrupts();
	tim1.setPrescaler(Timer16Bit::PrescaleOption::_1);
	tim1.setPeriod(160);
	tim1.start();
}