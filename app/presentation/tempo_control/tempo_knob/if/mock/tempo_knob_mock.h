/*
****************************************************************************************************
* brief : Interface for input device giving current tempo to set
****************************************************************************************************
*/

#ifndef TEMPO_KNOB_MOCK_H
#define TEMPO_KNOB_MOCK_H

#include "gmock/gmock.h"
#include "tempo_knob.h"

class TempoKnobMock : public TempoKnob
{
public:
	MOCK_CONST_METHOD0(getCurrentTempo, BeatsPerMinute());
};

#endif /* TEMPO_KNOB_MOCK_H */

