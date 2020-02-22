/*
****************************************************************************************************
* brief : This file contains the instantiation of all classes used in the application, and provides
*         an interface for accessing references to selected classes needed in main.
****************************************************************************************************
*/

#ifndef STARTUP_2_H
#define STARTUP_2_H

#include "TempoControlView.h"
#include "RhythmPlaybackManager.h"
#include "LedGroup.h"
#include "ButtonGroup.h"
#include "GpioMatrix.h"
#include "GpioPin.h"
#include "CallbackScheduler.h"
#include "MatrixMappedButtonGroup.h"

namespace Startup
{

void init();
LedGroup& getStepLeds();
ButtonGroup& getStepButtons();
CallbackScheduler& getCallbackScheduler();
RhythmPlaybackManager& getRhythmPlaybackManager();
MatrixMappedButtonGroup<GpioPin>& getTransportButtons();
RhythmPlaybackController& getPlaybackController();

};

#endif /* STARTUP_2_H */
