/*
****************************************************************************************************
* brief : This file contains the instantiation of all classes used in the application, and provides
*         an interface for accessing references to selected classes needed in main.
****************************************************************************************************
*/

#ifndef STARTUP_H
#define STARTUP_H

#include "tempo_control_view.h"
#include "tempotimingmanager.h"

namespace Startup
{

void init();
TempoControlView& getTempoControlView();
TempoTimingManager& getTempoTimingManager();

};

#endif /* STARTUP_H */
