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
#include "r2k/vector.h"

/**
 * @brief Called once every playback step (16th note)
 */
using PlaybackStepHandler = void(*)();

/**
 * @brief Called when playback is reset, i.e. start button pressed
 */
using ResetPlaybackHandler = void(*)();

class RhythmPlaybackManager
{
public:
	RhythmPlaybackManager(TempoTimer& tempoTimer);
	void addPlaybackStepHandler(PlaybackStepHandler handler);
	void addPlaybackResetHandler(ResetPlaybackHandler handler);
	void restartPlayback();
	void continuePlayback();
	void stopPlayback();
	void handlePlayback();
	u8 getPlaybackPosition() const;
	bool playbackIsOngoing();

	static constexpr u8 maxNumHandlers = 16;

private:
	TempoTimer& tempoTimer;
	bool isPlaying = false;
	u8 currentNumHandlers = 0;
	u8 playbackPosition = 0;
	r2k::vector<PlaybackStepHandler, maxNumHandlers> playbackStepHandlers;
	r2k::vector<ResetPlaybackHandler, maxNumHandlers> resetPlaybackHandlers;

	void callPlaybackStephandlers();
};

#endif /* RHYTHM_PLAYBACK_MANAGER */
