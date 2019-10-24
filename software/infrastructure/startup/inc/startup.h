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
#include "charlieplex_matrix.h" // only temporary
#include "gpiopin.h" // only temporary

namespace Startup
{

void init();
TempoControlView& getTempoControlView();
TempoTimingManager& getTempoTimingManager();
CharlieplexMatrix<GpioPin>& getLedMatrix();

};

#endif /* STARTUP_H */
