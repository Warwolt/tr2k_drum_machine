/*
****************************************************************************************************
* brief : Implementation of TempoKnob interface using a RotaryEncoder
****************************************************************************************************
*/

#ifndef DIGITAL_TEMPO_KNOB_H
#define DIGITAL_TEMPO_KNOB_H

#include "linuxtypes.h"
#include "TempoKnob.h"
#include "RotaryEncoder.h"

template<typename IGpioPin>
class DigitalTempoKnob : public TempoKnob
{
public:
	DigitalTempoKnob(RotaryEncoder<IGpioPin>& rotaryEncoder);
	void setReferenceTempo(u8 bpm);
	BeatsPerMinute read() const final;

private:
	static constexpr u8 minimumTempo = 60;
	static constexpr u8 maximumTempo = 200;
	static constexpr u8 initialReferenceTempo = 120;
	u8 referenceTempo = initialReferenceTempo;
	RotaryEncoder<IGpioPin>& rotaryEncoder;
};

#endif /* DIGITAL_TEMPO_KNOB_H */

