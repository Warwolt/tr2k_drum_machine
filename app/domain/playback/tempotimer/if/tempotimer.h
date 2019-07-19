/*
****************************************************************************************************
* brief : TempoTimer interface, responsible for abstracting a hardware timer into an application
*         that is able to signal when the next (16th note) playback step should be executed.
****************************************************************************************************
*/

#ifndef TEMPO_TIMER_H
#define TEMPO_TIMER_H

#include "beatsperminute.h"

class TempoTimer
{
public:
	virtual ~TempoTimer() {}
	virtual void setTempo(BeatsPerMinute bpm) = 0;
	virtual void start() = 0;
	virtual void stop() = 0;
	virtual void clear() = 0;
	virtual bool playbackStepIsDue() = 0;
	virtual void startCountingNextStep() = 0;
};

#endif /* TEMPO_TIMER_H */
