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
 * @brief Start playback by resetting tempo timer and all playback handlers
 */
void RhythmPlaybackManager::startPlayback()
{
	tempoTimer.clear();
	tempoTimer.start();
	isPlaying = true;
	// TODO: write test for resetting all playback handlers
}

/**
 * @brief Stops playback by halting the tempo timer
 */
void RhythmPlaybackManager::stopPlayback()
{
	tempoTimer.stop();
	isPlaying = false;
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
		// TODO: wrap this around the length of the active pattern instead of 16
		playbackPosition = (playbackPosition + 1) % 16;
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

/**
 * @brief Return the playback position of the active pattern
 */
u8 RhythmPlaybackManager::getPlaybackPosition() const
{
	return playbackPosition;
}

/**
 * @brief  Used to query if playback is stopped or not
 */
bool RhythmPlaybackManager::playbackIsOngoing()
{
	return isPlaying;
}
