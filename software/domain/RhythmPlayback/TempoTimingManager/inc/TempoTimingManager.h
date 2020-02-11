/*
****************************************************************************************************
* brief : Manages the timing of PlaybackStepHandlers by polling a TempoTimer instance for when
*         playback steps (16th notes) are due to be processed.
****************************************************************************************************
*/

#ifndef TEMPO_TIMING_MANAGER_H
#define TEMPO_TIMING_MANAGER_H

#include "linuxtypes.h"
#include "r2k/function.h"
#include "r2k/vector.h"
#include "TempoTimer.h"

/**
 * Callback function that executes some playback related code.
 * Called once every playback step (16th note).
 */
using PlaybackStepHandler = r2k::function<void()>;

class TempoTimingManager
{
public:
	TempoTimingManager(TempoTimer& tempoTimer);
	void addPlaybackStepHandler(PlaybackStepHandler handler);
	void handlePlayback();

	static constexpr u8 maxNumHandlers = 16;

private:
	void callPlaybackStephandlers();

	TempoTimer& tempoTimer;
	r2k::vector<PlaybackStepHandler, maxNumHandlers> playbackStepHandlers;
};

#endif /* TEMPO_TIMING_MANAGER_H */
