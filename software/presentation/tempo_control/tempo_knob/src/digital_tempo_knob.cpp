/*
****************************************************************************************************
* brief : Driver for the rotary encoder.
****************************************************************************************************
*/

#include "digital_tempo_knob.h"
#include "gpiopin.h"

#ifdef UNIT_TESTING
#include "igpiopin_mock.h"
#endif

template<typename IGpioPin>
DigitalTempoKnob<IGpioPin>::DigitalTempoKnob(RotaryEncoder<IGpioPin>& encoder)
	: rotaryEncoder(encoder)
{

}

template<typename IGpioPin>
void DigitalTempoKnob<IGpioPin>::setReferenceTempo(u8 bpm)
{
	referenceTempo = bpm;
}

template<typename IGpioPin>
BeatsPerMinute DigitalTempoKnob<IGpioPin>::read() const
{
	s16 currentTempo = referenceTempo + rotaryEncoder.getNumRotations();

	if(currentTempo < minimumTempo)
	{
		currentTempo = minimumTempo;
	}

	if(currentTempo > maximumTempo)
	{
		currentTempo = maximumTempo;
	}

	return BeatsPerMinute(static_cast<u8>(currentTempo));
}

/* Explicit instantiation of template to avoid linking errors */
template class DigitalTempoKnob<GpioPin>;

#ifdef UNIT_TESTING
template class DigitalTempoKnob<GpioPinMock>;
#endif
