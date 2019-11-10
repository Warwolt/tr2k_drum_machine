/*
****************************************************************************************************
* brief : Interface for input device giving current tempo to set
****************************************************************************************************
*/

#ifndef TEMPO_KNOB_MOCK_H
#define TEMPO_KNOB_MOCK_H

#include "gmock/gmock.h"
#include "TempoKnob.h"

class TempoKnobMock : public TempoKnob
{
public:
	MOCK_CONST_METHOD0(read, BeatsPerMinute());
};

#endif /* TEMPO_KNOB_MOCK_H */

