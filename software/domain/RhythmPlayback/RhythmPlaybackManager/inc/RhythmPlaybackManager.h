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
 * @brief Represents an image of a rhythm pattern
 *
 * @param handlePlaybackStep  Called once every playback step (16th note)
 * @param resetPlayback       Called when playback is reset, i.e. start button pressed
 */
using CallbackFunction = void(*)();
struct PlaybackHandler
{
	CallbackFunction handlePlaybackStep;
	CallbackFunction resetPlayback;
};

class RhythmPlaybackManager
{
public:
	RhythmPlaybackManager(TempoTimer& tempoTimer);
	void addPlaybackHandler(PlaybackHandler handler);
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
	PlaybackHandler playbackHandlers[maxNumHandlers]; // TODO: test if r2k::vector based version works on-target

	void callPlaybackStephandlers();
};

#endif /* RHYTHM_PLAYBACK_MANAGER */
