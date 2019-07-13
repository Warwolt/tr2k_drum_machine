#include "gpiopin.h"
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

GpioPin ledPin = GpioPin(PIN_5, PORT_B, DIGITAL_OUTPUT);

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

	/* trial code: initialize timer 1 */
	// set mode to Clear on Compare Match (CTC)
	TCCR1B |= 0x1 << WGM12;

	// enable periodic timer 1 interrupts
	TIMSK1 |= 0x1 << OCIE1A;

	// set period
	OCR1A = 160 - 1; // with 16 MHz clock this should give a 10 microsecond period

	// select prescaler and start timer
	TCCR1B |= 0x1 << CS10;
}