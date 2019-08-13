/*
****************************************************************************************************
* brief : UI controller for rhythm playback (start, stop, set tempo etc.)
****************************************************************************************************
*/

#ifndef RHYTHM_PLAYBACK_CONTROLLER_H
#define RHYTHM_PLAYBACK_CONTROLLER_H

#include "linuxtypes.h"
#include "tempotimer.h"

class RhythmPlaybackController
{
public:
	RhythmPlaybackController(TempoTimer& timer);
	void setTempo(BeatsPerMinute);
	void restartPlayback();
	void stopPlayback();
	void continuePlayback();

private:
	TempoTimer& tempoTimer;
};

#endif /* RHYTHM_PLAYBACK_CONTROLLER_H */
