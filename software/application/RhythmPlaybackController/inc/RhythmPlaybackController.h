/*
****************************************************************************************************
* brief : UI controller for rhythm playback (start, stop, set tempo etc.)
****************************************************************************************************
*/

#ifndef RHYTHM_PLAYBACK_CONTROLLER_H
#define RHYTHM_PLAYBACK_CONTROLLER_H

#include "linuxtypes.h"
#include "TempoTimer.h"
#include "RhythmPlaybackManager.h"

class RhythmPlaybackController
{
public:
	RhythmPlaybackController(RhythmPlaybackManager& manager, TempoTimer& timer);
	void setTempo(BeatsPerMinute);
	void restartPlayback();
	void stopPlayback();
	void continuePlayback();
	bool playbackIsOngoing();

private:
	RhythmPlaybackManager& playbackManager;
	TempoTimer& tempoTimer;
};

#endif /* RHYTHM_PLAYBACK_CONTROLLER_H */
