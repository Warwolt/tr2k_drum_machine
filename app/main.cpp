#include "gpiopin.h"
#include <util/delay.h>

GpioPin ledPin = GpioPin(PIN_5, PORT_B, DIGITAL_OUTPUT);

void init();

int main()
{
	init();

	while(1)
	{
		ledPin.toggle();
	}
}

void init()
{
	// empty init;
}