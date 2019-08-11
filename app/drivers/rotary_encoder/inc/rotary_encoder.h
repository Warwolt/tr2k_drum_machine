/*
****************************************************************************************************
* brief : Driver for rotary encoder.
****************************************************************************************************
*/

#ifndef ROTARY_ENCODER_H
#define ROTARY_ENCODER_H

#include "linuxtypes.h"
#include <avr/io.h>

template<typename IGpioPin>
class RotaryEncoder
{
public:
	RotaryEncoder(IGpioPin& pinA, IGpioPin& pinB);
	void handleEdge();
	s16 getNumRotations();

private:
	s16 numRotations = 0;
	IGpioPin& pinA;
	IGpioPin& pinB;
};

#endif /* ROTARY_ENCODER_H */
