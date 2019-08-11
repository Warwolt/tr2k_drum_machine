/*
****************************************************************************************************
* brief : Driver for the rotary encoder.
****************************************************************************************************
*/

#include "rotary_encoder.h"
#include "gpiopin.h"

#ifdef UNIT_TESTING
#include "igpiopin_mock.h"
#endif

template<typename IGpioPin>
RotaryEncoder<IGpioPin>::RotaryEncoder(IGpioPin& pinA, IGpioPin& pinB) : pinA(pinA), pinB(pinB)
{

}

template<typename IGpioPin>
s16 RotaryEncoder<IGpioPin>::getNumRotations()
{
	return numRotations;
}

template<typename IGpioPin>
void RotaryEncoder<IGpioPin>::handleEdge()
{
	if(pinA.read() == LogicState::Low)
	{
		if(pinB.read() == LogicState::Low)
		{
			numRotations++;
		}
		else
		{
			numRotations--;
		}
	}
}

/* Explicit instantiation of template to avoid linking errors */
template class RotaryEncoder<GpioPin>;

#ifdef UNIT_TESTING
template class RotaryEncoder<GpioPinMock>;
#endif
