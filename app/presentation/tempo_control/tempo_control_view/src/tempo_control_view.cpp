/*
****************************************************************************************************
* brief : Interface view for controlling the playback tempo
****************************************************************************************************
*/

#include "tempo_control_view.h"

TempoControlView::TempoControlView(RhythmPlaybackController& controller, TempoKnob& knob,
		FourDigitDisplay& display) : controller(controller), knob(knob), display(display)
{
	display.enableDecimalPoint(1); // make display format "120.0", one decimal digit
}

void TempoControlView::handleTempoControl()
{
	BeatsPerMinute bpm = knob.read();
	display.setNumberToDisplay(bpm * 10);
	controller.setTempo(bpm);
}
