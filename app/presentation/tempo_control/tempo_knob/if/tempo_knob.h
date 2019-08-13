/*
****************************************************************************************************
* brief : Interface for input device giving current tempo to set
****************************************************************************************************
*/

#ifndef TEMPO_KNOB_H
#define TEMPO_KNOB_H

#include "beatsperminute.h"

class TempoKnob
{
public:
	virtual ~TempoKnob(){}
	virtual BeatsPerMinute read() const = 0;
};

#endif /* TEMPO_KNOB_H */

