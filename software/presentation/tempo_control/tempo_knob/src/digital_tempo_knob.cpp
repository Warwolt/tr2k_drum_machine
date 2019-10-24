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

#include "traceprint.h"

template<typename IGpioPin>
DigitalTempoKnob<IGpioPin>::DigitalTempoKnob(RotaryEncoder<IGpioPin>& encoder)
	: rotaryEncoder(encoder)
{
	// these two lines don't have unit tests but not really worth the hassle to
	// make worth testing so I'm skipping it. (Already tested that methods work
	// in rotary encoder unit tests!)
	encoder.setRotationCeiling((maximumTempo - (s16)initialReferenceTempo));
	encoder.setRotationFloor((minimumTempo - (s16)initialReferenceTempo));
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
	return BeatsPerMinute(static_cast<u8>(currentTempo));
}

/* Explicit instantiation of template to avoid linking errors */
template class DigitalTempoKnob<GpioPin>;

#ifdef UNIT_TESTING
template class DigitalTempoKnob<GpioPinMock>;
#endif
