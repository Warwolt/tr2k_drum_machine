/*
****************************************************************************************************
* brief : Driver for rotary encoder.
****************************************************************************************************
*/

#ifndef ROTARY_ENCODER_H
#define ROTARY_ENCODER_H

#include "linuxtypes.h"

template<typename IGpioPin>
class RotaryEncoder
{
public:
	RotaryEncoder(IGpioPin& pinA, IGpioPin& pinB);
	void handleEdge();
	void setRotationCeiling(s16);
	void setRotationFloor(s16);
	s16 getNumRotations();

private:
	s16 numRotations = 0;
	s16 rotationCeiling = 32767;
	s16 rotationFloor = -32768;
	IGpioPin& pinA;
	IGpioPin& pinB;
};

#endif /* ROTARY_ENCODER_H */
