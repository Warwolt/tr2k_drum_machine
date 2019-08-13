/*
****************************************************************************************************
* brief : Interface view for controlling the playback tempo
****************************************************************************************************
*/

#include "tempo_control_view.h"

TempoControlView::TempoControlView(RhythmPlaybackController& controller, TempoKnob& knob,
		FourDigitDisplay& display) : controller(controller), knob(knob), display(display)
{

}

void TempoControlView::handleTempoControl()
{
	BeatsPerMinute bpm = knob.read();
	display.setNumberToDisplay(bpm);
	controller.setTempo(bpm);
}
