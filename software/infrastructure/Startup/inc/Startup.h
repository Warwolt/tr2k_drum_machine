/*
****************************************************************************************************
* brief : This file contains the instantiation of all classes used in the application, and provides
*         an interface for accessing references to selected classes needed in main.
****************************************************************************************************
*/

#ifndef STARTUP_2_H
#define STARTUP_2_H

#include "TempoControlView.h"
#include "TempoTimingManager.h"
#include "BlinkableLedGroup.h"
#include "ButtonGroup.h"
#include "GpioMatrix.h"
#include "GpioPin.h"
#include "CallbackScheduler.h"

namespace Startup
{

void init();
BlinkableLedGroup& getStepLeds();
ButtonGroup& getStepButtons();
CallbackScheduler& getCallbackScheduler();

};

#endif /* STARTUP_2_H */
