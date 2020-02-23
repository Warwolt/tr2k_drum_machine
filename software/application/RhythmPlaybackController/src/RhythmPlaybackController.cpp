/*
****************************************************************************************************
* brief : UI controller for rhythm playback (start, stop, set tempo etc.)
****************************************************************************************************
*/

#include "RhythmPlaybackController.h"
#include "TempoKnob.h"
#include "FourDigitDisplay.h"

RhythmPlaybackController::RhythmPlaybackController(RhythmPlaybackManager& manager,
	TempoTimer& timer) : playbackManager(manager), tempoTimer(timer) {}

void RhythmPlaybackController::setTempo(BeatsPerMinute bpm)
{
	tempoTimer.setTempo(bpm);
}

void RhythmPlaybackController::restartPlayback()
{
	playbackManager.startPlayback();
}

void RhythmPlaybackController::stopPlayback()
{
	playbackManager.stopPlayback();
}

void RhythmPlaybackController::continuePlayback()
{
	playbackManager.continuePlayback();
}

bool RhythmPlaybackController::playbackIsOngoing()
{
	return playbackManager.playbackIsOngoing();
}
