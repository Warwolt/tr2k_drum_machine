/*
****************************************************************************************************
* brief : Manages the timing of PlaybackStepHandlers by polling a TempoTimer instance for when
*         playback steps (16th notes) are due to be processed.
****************************************************************************************************
*/

#ifndef RHYTHM_PLAYBACK_MANAGER
#define RHYTHM_PLAYBACK_MANAGER

#include "linuxtypes.h"
#include "TempoTimer.h"

/**
 * Callback function that executes some playback related code.
 * Called once every playback step (16th note).
 */
using PlaybackStepHandler = void(*)();

class RhythmPlaybackManager
{
public:
	RhythmPlaybackManager(TempoTimer& tempoTimer);
	void addPlaybackStepHandler(PlaybackStepHandler handler);
	void handlePlayback();

	static constexpr u8 maxNumHandlers = 16;

private:
	void callPlaybackStephandlers();

	TempoTimer& tempoTimer;
	u8 currentNumHandlers = 0;
	PlaybackStepHandler playbackStepHandlers[maxNumHandlers];
};

#endif /* RHYTHM_PLAYBACK_MANAGER */
