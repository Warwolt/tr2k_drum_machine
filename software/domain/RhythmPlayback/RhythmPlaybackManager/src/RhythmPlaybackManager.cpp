/*
****************************************************************************************************
* brief : Manages the timing of TempoTimingSubscribers by polling a TempoTimer instance for when
*         playback steps are due.
****************************************************************************************************
*/

#include "RhythmPlaybackManager.h"
#include <stddef.h>

RhythmPlaybackManager::RhythmPlaybackManager(TempoTimer& tempoTimer) : tempoTimer(tempoTimer) {}

/**
 * @brief Start playback by resetting tempo timer and all playback handlers
 */
void RhythmPlaybackManager::restartPlayback()
{
	isPlaying = true;

	/* Restart tempo timer */
	tempoTimer.clear();
	tempoTimer.start();

	/* Reset playback position and playback handlers */
	playbackPosition = 0;
	for (auto& resetPlayback : resetPlaybackHandlers)
	{
		resetPlayback();
	}
}

/**
 * @brief Stops playback by halting the tempo timer
 */
void RhythmPlaybackManager::stopPlayback()
{
	isPlaying = false;
	tempoTimer.stop();
}

/**
 * @brief Continue playback wihtout restarting it
 */
void RhythmPlaybackManager::continuePlayback()
{
	tempoTimer.start();
	isPlaying = true;
}

/**
 * @brief Register function to be called once every playback step
 */
void RhythmPlaybackManager::addPlaybackStepHandler(PlaybackStepHandler handler)
{
	if (playbackStepHandlers.size() < playbackStepHandlers.capacity())
	{
		playbackStepHandlers.push_back(handler);
	}
}

/**
 * @brief Register function to be called when playback restarts
 */
void RhythmPlaybackManager::addPlaybackResetHandler(ResetPlaybackHandler handler)
{
	if (resetPlaybackHandlers.size() < resetPlaybackHandlers.capacity())
	{
		resetPlaybackHandlers.push_back(handler);
	}
}


/**
 * @brief Iterate handling of playback one step
 *
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
	for (auto& handlePlayback : playbackStepHandlers)
	{
		handlePlayback();
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
