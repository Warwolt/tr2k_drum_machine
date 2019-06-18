#include "gpiopin.h"
#include <util/delay.h>

GpioPin ledPin = GpioPin(PIN_5, PORT_B, DIGITAL_OUTPUT);

void init();

int main()
{
	init();

	while(1)
	{
		// ledPin.toggle();
		// _delay_ms(100);
	}
}

void init()
{
	// empty init
}