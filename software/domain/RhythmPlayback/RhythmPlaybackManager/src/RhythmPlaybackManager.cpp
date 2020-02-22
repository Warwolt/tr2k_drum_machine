/*
****************************************************************************************************
* brief : Manages the timing of TempoTimingSubscribers by polling a TempoTimer instance for when
*         playback steps are due.
****************************************************************************************************
*/

#include "RhythmPlaybackManager.h"
#include <stddef.h>

RhythmPlaybackManager::RhythmPlaybackManager(TempoTimer& tempoTimer) : tempoTimer(tempoTimer)
{

}

/**
 * Add a new callback to be called in the handlePlayback() method.
 * If maximum number of callbacks registered, this method does nothing.
 */
void RhythmPlaybackManager::addPlaybackStepHandler(PlaybackStepHandler handler)
{
	if (currentNumHandlers < maxNumHandlers)
	{
		playbackStepHandlers[currentNumHandlers++] = handler;
	}
}

/**
 * Checks with the TempoTimer if a playback step is due for execution.
 * If it is, all registered playback step handler callbacks are called.
 */
void RhythmPlaybackManager::handlePlayback()
{
	if (tempoTimer.playbackStepIsDue())
	{
		callPlaybackStephandlers();
		tempoTimer.startCountingNextStep();
	}
}

inline void RhythmPlaybackManager::callPlaybackStephandlers()
{
	for (size_t i = 0; i < currentNumHandlers; i++)
	{
		auto& handlePlaybackStep = playbackStepHandlers[i];
		handlePlaybackStep();
	}
}
