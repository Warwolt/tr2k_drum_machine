/*
****************************************************************************************************
* brief : Interface view for controlling the playback tempo
****************************************************************************************************
*/

#ifndef TEMPO_CONTROL_VIEW_H
#define TEMPO_CONTROL_VIEW_H

#include "rhythm_playback_controller.h"
#include "TempoKnob.h"
#include "fourdigitdisplay.h"

class TempoControlView
{
public:
	TempoControlView(RhythmPlaybackController& controller, TempoKnob& knob,
		FourDigitDisplay& display);
	void handleTempoControl();

private:
	RhythmPlaybackController& controller;
	TempoKnob& knob;
	FourDigitDisplay& display;
};

#endif /* TEMPO_CONTROL_VIEW_H */

