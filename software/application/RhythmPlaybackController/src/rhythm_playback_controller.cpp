/*
****************************************************************************************************
* brief : UI controller for rhythm playback (start, stop, set tempo etc.)
****************************************************************************************************
*/

#include "rhythm_playback_controller.h"
#include "TempoKnob.h"
#include "fourdigitdisplay.h"

RhythmPlaybackController::RhythmPlaybackController(TempoTimer& timer) : tempoTimer(timer)
{

}

void RhythmPlaybackController::setTempo(BeatsPerMinute bpm)
{
	tempoTimer.setTempo(bpm);
}

void RhythmPlaybackController::restartPlayback()
{
	tempoTimer.clear(); // resets timer counter
	tempoTimer.start(); // restarts counting
}

void RhythmPlaybackController::stopPlayback()
{
	tempoTimer.stop(); // stop counting, only pauses playback
}

void RhythmPlaybackController::continuePlayback()
{
	tempoTimer.start(); // start counting again without clearing counter
}
